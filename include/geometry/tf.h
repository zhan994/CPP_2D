/**
 * \file tf.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 坐标转换
 * \version 0.1
 * \date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GEOMETRY_TF_H
#define GEOMETRY_TF_H

#include "bg.h"

class TF {
  public:
  /**
   * \brief 绕中心点旋转 Point
   *
   * \param pt 点
   * \param center 中心
   * \param angle 角度deg
   * \return Point
   */
  static Point Rotate(const Point &pt, const Point &center, double angle);

  /**
   * \brief 绕中心点旋转 Segment
   *
   * \param seg 线段
   * \param center 中心
   * \param angle 角度deg
   * \return Segment
   */
  static Segment Rotate(const Segment &seg, const Point &center, double angle);

  /**
   * \brief 绕中心点旋转 Polygon
   *
   * \param pg 多边形
   * \param center 中心
   * \param angle 角度deg
   * \return Polygon
   */
  static Polygon Rotate(const Polygon &pg, const Point &center, double angle);
};

#endif //  GEOMETRY_TF_H