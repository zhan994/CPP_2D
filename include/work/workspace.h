/**
 * \file work.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 工作区
 * \version 0.1
 * \date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WORK_WORKSPACE_H
#define WORK_WORKSPACE_H

#include "geometry/utility.h"

#include <iostream>
#include <chrono>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::listS,
                              boost::vecS,
                              boost::undirectedS,
                              boost::property<boost::vertex_name_t, Point>,
                              boost::property<boost::edge_weight_t, double>>
                                                               Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor          Vertex;
typedef boost::graph_traits<Graph>::edge_descriptor            Edge;
typedef boost::property_map<Graph, boost::vertex_name_t>::type Vertices;
typedef boost::property_map<Graph, boost::edge_weight_t>::type EdgeWeights;

enum WaypointType { NORMAL_START, NORMAL_END, TRANSITION, DETOUR };

class Trapezoid {
  public:
  Trapezoid(const Polygon &p, const Segment &left, const Segment &right);

  Polygon polygon_;
  Segment left_, right_;
  double  left_x_, right_x_;
  bool    left_state_, right_state_; // 起点和终点一致则为false
};

class Work {
  public:
  /**
   * \brief 空构造
   *
   */
  Work();

  /**
   * \brief 设置区域数据
   *
   * \param outline 地块边界点
   * \param holes 地块障碍区点
   */
  void SetPolygon(const std::vector<Point> &outline, const std::vector<std::vector<Point>> &holes);

  /**
   * \brief 获取工作区
   *
   * \return Polygon
   */
  Polygon GetPolygon()
  {
    return work_pg_;
  }

  /**
   * \brief 获取中心
   *
   * \return Point
   */
  Point GetCenter()
  {
    return centroid_;
  }

  /**
   * \brief 地块旋转
   *
   * \param angle 旋转角度
   */
  void RotateIt(double angle);

  /**
   * \brief 添加点信息
   *
   * \param pt_str
   * \param edge_str
   */
  void Register(const std::string &pt_str, const std::string &edge_str);

  /**
   * \brief 求最短路径
   *
   * \param start 起点
   * \param end 终点
   * \param path 输出路径
   * \return double 路径长度
   */
  double ShortestPath(const Point &start, const Point &end, std::vector<Point> &path);

  /**
   * \brief 计算tracks
   *
   * \param start 起点
   * \param interval 间隔
   * \param offset 起始偏置
   * \param out　输出tracks
   *
   * \return double 起始航线x坐标
   */
  double GetTracks(const Point &start_pt, double interval, double offset, std::vector<Segment> &out);

  /**
   * \brief 获取分割区域块
   *
   * \param start_point 起点
   * \param out 输出
   */
  void GetMonotoneZones(const Point &start_pt, std::vector<Trapezoid> &out);

  private:
  /**
   * \brief 添加障碍区
   *
   * \param hole 障碍区点
   */
  void AddHole(const std::vector<Point> &hole);

  /**
   * \brief 记录点边表
   *
   */
  void RegisterMap();

  /**
   * \brief 创建图
   *
   */
  void InitGraph();

  /**
   * \brief 验证点点线段是否有效
   *
   * \param pt1
   * \param pt2
   * \return true
   * \return false
   */
  bool ValidPtsPair(const Point &pt1, const Point &pt2);

  /**
   * \brief 判断两点是否在同一条边
   *
   * \param pt1
   * \param pt2
   * \return true
   * \return false
   */
  bool PointsOnSameEdge(const Point &pt1, const Point &pt2);

  /**
   * \brief dijkstra最短路径
   *
   * \param start_id
   * \param end_id
   * \param path
   * \return double
   */
  double Dijkstra(const std::string &start_id, const std::string &end_id, std::vector<Point> &path);

  /**
   * \brief 垂直方向线段
   *
   * \param start_pt
   * \param out
   */
  void VerticalSweepLines(const Point &start_pt, std::vector<Segment> &out);

  Polygon work_pg_;
  Point   centroid_;

  std::vector<Point>                all_pts_;
  std::vector<std::string>          all_pts_id_;
  std::vector<Segment>              outline_segs_;
  std::vector<std::vector<Segment>> holes_segs_;
  std::vector<Segment>              all_segs_;

  // Stores points on polygons and their belongs polygon edges. Each edge is named
  // according to its polygon order id and its edge order id. For example, <0, 0>
  // means the 1st edge of the outline polygon; and <1, 8> represents the 9th edge of the
  // 1st hole polygon.
  std::unordered_map<std::string, std::vector<std::string>> registered_pts_map_;

  // Stores point pairs and a boolean number denoting if their connection segment is a valid
  // path inside the working area. This info is frequently used, hence storing it saves time.
  std::unordered_map<std::string, bool> registered_pts_pairs_;

  Graph                                   graph_, graph_bak_;
  std::unordered_map<std::string, Vertex> graph_map_;
};

#endif // WORK_WORKSPACE_H