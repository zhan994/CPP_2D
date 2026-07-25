/**
 * \file cell.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief A cell represents a group of tracks.
 * \version 0.1
 * \date 2023-06-11
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WORK_CELL_H
#define WORK_CELL_H

#include "workspace.h"

class Cell {
  public:
  /**
   * \brief 构造
   *
   */
  Cell();

  /**
   * \brief 添加航线
   *
   * \param track
   */
  void AddTrack(const Segment &track);

  /**
   * \brief 航线排序
   *
   */
  void SortTracks();

  /**
   * \brief 节点数量
   *
   * \return int
   */
  int NumNodes();

  /**
   * \brief 设置单点
   *
   * \param pt
   */
  void SetSinglePoint(const Point &pt);

  /**
   * \brief 获取路径端点
   *
   * \param ni
   * \param out
   */
  void GetPathEnd(size_t ni, std::vector<Point> &out);

  /**
   * \brief 计算路径
   *
   * \param work
   * \param simplified
   */
  void GetPaths(Work &work, bool simplified = false);

  /**
   * \brief 计算路径
   *
   * \param working_area
   * \param from_left
   * \param from_up
   * \param simplified
   *
   * \return std::vector<Point>
   */
  std::vector<Point> GetPaths(Work &work, bool from_left, bool from_up, bool simplified = false);

  std::vector<Segment>            tracks_, sorted_tracks_;
  std::vector<std::vector<Point>> paths_;

  // Stores the length values of each node. Each node should has two length values:
  // 1st: length of the whole path; 2nd: total length of the connection segments connecting
  // adjacent tracks.
  std::vector<std::pair<double, double>> paths_length_;
  std::vector<std::vector<WaypointType>> waypoint_types_;

  // If sets, it means the cell only contains a single point.
  // This is usually for the start point.
  Point single_pt_;
  bool  single_pt_state_;
};

#endif // WORK_CELL_H