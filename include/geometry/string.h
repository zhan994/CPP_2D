/**
 * \file string.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 几何 字符串
 * \version 0.1
 * \date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GEOMETRY_STRING_H
#define GEOMETRY_STRING_H

#include "bg.h"

class String {
  public:
  /**
   * \brief 点转字符串
   *
   * \param pt
   * \return std::string
   */
  static std::string PointToString(const Point &pt);

  /**
   * \brief 线段转字符串
   *
   * \param seg
   * \return std::string
   */
  static std::string SegmentToString(const Segment &seg);
};

#endif // GEOMETRY_STRING_H