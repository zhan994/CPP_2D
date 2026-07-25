/**
 * \file bool.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d几何的布尔运算
 * \version 0.1
 * \date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GEOMETRY_BOOL_H
#define GEOMETRY_BOOL_H

#include "bg.h"

class Bool {
  public:
  /**
   * \brief 线段之间的交集
   *
   * \param seg1
   * \param seg2
   * \param out
   */
  static void Intersection(const Segment &seg1, const Segment &seg2, std::vector<Point> &out);

  /**
   * \brief 线段与多边形的交集
   *
   * \param seg
   * \param pg
   * \param out
   */
  static void Intersection(const Segment &seg, const Polygon &pg, std::vector<Point> &out);

  /**
   * \brief 多边形之间的交集
   *
   * \param pg1
   * \param pg2
   * \param out
   */
  static void Intersection(const Polygon &pg1, const Polygon &pg2, std::vector<Polygon> &out);

  /**
   * \brief pg1 - pg2
   *
   * \param pg1
   * \param pg2
   * \param out
   */
  static void Difference(const Polygon &pg1, const Polygon &pg2, std::vector<Polygon> &out);

  /**
   * \brief pg1 + pg2
   *
   * \param pg1
   * \param pg2
   * \param out
   */
  static void Union(const Polygon &pg1, const Polygon &pg2, std::vector<Polygon> &out);

  /**
   * \brief 线段是否被多边形包含，包括在边上
   *
   * \param seg
   * \param pg
   * \return true
   * \return false
   */
  static bool CoveredBy(const Segment &seg, const Polygon &pg);

  /**
   * \brief 线段是否被多边形包含，不包括在边上
   *
   * \param seg
   * \param pg
   * \return true
   * \return false
   */
  static bool Within(const Segment &seg, const Polygon &pg);
};

#endif // GEOMETRY_BOOL_H