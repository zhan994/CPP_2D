/**
 * \file buffer.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 几何收缩外扩
 * \version 0.1
 * \date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GEOMETRY_BUFFER_H
#define GEOMETRY_BUFFER_H

#include "bg.h"

class Buffer {
  public:
  /**
   * \brief 收缩 Segment
   *
   * \param seg
   * \return Segment
   */
  static Segment Shrink(const Segment &seg);

  /**
   * \brief 收缩 Polygon
   *
   * \param pg
   * \param out
   *
   * \return true
   * \return false
   */
  static bool Shrink(const Polygon &pg, Polygon &out);

  /**
   * \brief 多边形bbox
   *
   * \param pg
   * \param bbox min_x, max_x, min_y, max_y
   */
  static void BBox(const Polygon &pg, std::vector<double> &bbox);
};

#endif // GEOMETRY_BUFFER_H