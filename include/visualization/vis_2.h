/**
 * \file vis_2.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d几何可视化
 * \version 0.1
 * \date 2023-05-24
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef VISUALIZATION_GEOMETRY_VIS_2_H
#define VISUALIZATION_GEOMETRY_VIS_2_H

#include "geometry/utility_2.h"

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

class Vis_2 {
  public:
  Vis_2(std::string prefix = "/home/zhan/test_planning");

  /**
   * \brief 新建数据
   *
   * \param label 标签
   * \param data 数据
   * \param color 颜色
   */
  void NewFile(std::string label, const Polygon &data, std::string color="black");

  void NewFile(std::string label, const std::vector<Polygon> &data, std::string color="red");

  void NewFile(std::string label, const std::vector<Segment> &data, std::string color="yellow");

  void NewFile(std::string label, const std::vector<Point> &data, std::string color="blue");

  /**
   * \brief 清空数据
   *
   */
  void Clear();

  /**
   * \brief 执行
   *
   */
  void Show();

  private:
  /**
   * \brief 判断路径是否存在
   *
   * \param path_name
   * \return true
   * \return false
   */
  bool IsDirExist(const std::string &path_name);

  /**
   * \brief 创建新的路径
   *
   * \param path_name
   * \return true
   * \return false
   */
  bool CreateNewDir(const std::string &path_name);

  /**
   * \brief 判断文件是否存在
   *
   * \param file_name
   * \return true
   * \return false
   */
  bool IsFileExist(const std::string &file_name);

  std::vector<std::string> polygon_fn_, polygon_color_;
  std::vector<std::string> hole_fn_, hole_color_;
  std::vector<std::string> segment_fn_, segment_color_;
  std::vector<std::string> point_fn_, point_color_;
  std::string              prefix_;
};

#endif // VISUALIZATION_GEOMETRY_VIS_2_H