/**
 * \file route_2.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d航线规划接口
 * \version 0.1
 * \date 2023-05-25
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WORK_ROUTE_2_H
#define WORK_ROUTE_2_H

#include "cell_2.h"
#include "dfs_2.h"

#include <iostream>

class Route_2 {
  public:
  /**
   * \brief 构造
   *
   */
  Route_2();

  /**
   * \brief 设置工作地块
   *
   * \param outline 边界点
   * \param holes　障碍区点
   */
  void SetWork(const std::vector<Point> &outline, const std::vector<std::vector<Point>> &holes);

  /**
   * \brief 设置规划参数
   *
   * \param start_pt 起点
   * \param rotation 旋转角度
   * \param interval 行距
   * \param offset 起始偏置
   * \param preview 预览
   */
  void SetParam(const Point &start_pt, double rotation, double interval, double offset, bool preview);

  /**
   * \brief 规划
   *
   * \param path 输出路径
   * \param types 输出路径点类型
   * \param line_nums 输出路径编号
   */
  void PlanWithRotation(std::vector<Point> &path, std::vector<WaypointType> &types, std::vector<int> &line_nums);

  /**
   * \brief 终止dfs
   * 
   */
  void Stop();

  private:
  /**
   * \brief 将航线划分到cell中
   *
   * \param tracks
   * \param zones
   * \param out
   */
  void DivideTracksIntoCells(std::vector<Segment> &          tracks,
                             const std::vector<Trapezoid_2> &zones,
                             std::vector<Cell_2> &           out);

  /**
   * \brief pivotoal candi
   *
   * \param sorted_tracks
   * \param out
   */
  void CollectPivotalTrackCandidate(const std::vector<Segment> &sorted_tracks, std::vector<Segment> &out);

  /**
   * \brief start cell
   *
   * \param cells
   * \param start_point
   * \param start_x
   * \param sci
   * \param snii
   */
  void GetStartCellIndex(
      const std::vector<Cell_2> &cells, const Point &start_point, double start_x, size_t &sci, size_t &sni);

  /**
   * \brief 计算编号
   *
   * \param path
   * \param types
   * \param line_nums
   */
  void GetLineNums(const std::vector<Point> &path, const std::vector<WaypointType> &types, std::vector<int> &line_nums);

  std::vector<Point>              outline_;
  std::vector<std::vector<Point>> holes_;
  Work_2                          work_;
  Point                           origin_pt_;
  DFS_2                           dfs_;

  Point  start_pt_;
  double rotation_;
  double interval_;
  double offset_;
  bool   preview_, status_;
};

#endif // WORK_ROUTE_2_H