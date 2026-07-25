/**
 * \file dfs.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 基于 Work 和 Cell 的 DFS
 * \version 0.1
 * \date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef WORK_DFS_H
#define WORK_DFS_H

#include "cell.h"

class DFS {
  public:
  /**
   * \brief Construct a new dfs 2 object
   *
   */
  DFS();

  /**
   * \brief 构造
   *
   * \param work
   * \param cells
   * \param max_cost
   * \param cells_threshold
   */
  DFS(const Work &work, const std::vector<Cell> &cells, int max_cost, int cells_threshold);

  /**
   * \brief 求最优路径
   *
   * \param sci
   * \param sni
   * \param interval
   * \param path_out
   * \param wp_types_out
   */
  void FindMinimunPath(
      size_t sci, size_t sni, double interval, std::vector<Point> &path_out, std::vector<WaypointType> &wp_types_out);

  private:
  /**
   * \brief 获取权重
   *
   * \param ci1
   * \param ni1
   * \param ci2
   * \param ni2
   * \return double
   */
  double GetWeight(size_t ci1, size_t ni1, size_t ci2, size_t ni2);

  /**
   * \brief 求edge权重
   *
   * \param c_i1
   * \param n_i1
   * \param c_i2
   * \param n_i2
   *
   * \return double
   */
  double ComputeWeight(size_t ci1, size_t ni1, size_t ci2, size_t ni2);

  /**
   * \brief cell中node的个数
   *
   * \param c_i
   * \return int
   */
  int NumNodes(size_t c_i);

  /**
   * \brief DFS
   *
   * \param cur_ci
   * \param cur_ni
   * \param path
   * \param current_weight
   */
  void DFS_Search(size_t cur_ci, size_t cur_ni, std::vector<std::pair<size_t, size_t>> &path, double current_weight);

  /**
   * \brief 解析最终输出
   *
   * \param interval
   * \param path_out
   * \param wp_types_out
   */
  void TranslatePath(double interval, std::vector<Point> &path_out, std::vector<WaypointType> &wp_types_out);

  /**
   * \brief 求路径之间的链接关系
   *
   * \param cur_path
   * \param next_path
   * \param interval
   * \return std::string
   */
  std::string GetCellConnectionType(const std::vector<Point> &cur_path,
                                    const std::vector<Point> &next_path,
                                    double                    interval);

  Work              work_;
  std::vector<Cell> cells_;
  int               N_, cells_threshold_;
  int               max_cost_, cost_;
  std::vector<bool> visited_;

  // key: ci1:ni1_ci2:ni2,  val: dist btw. ci1:ni1 & ci2:ni2
  std::unordered_map<std::string, double> edge_weights_;

  // solution
  double best_solution_weight_;
  // first cell_index, second node_index
  std::vector<std::pair<size_t, size_t>> best_path_;
};

#endif // WORK_DFS_H