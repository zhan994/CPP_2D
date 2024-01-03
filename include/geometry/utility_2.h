/**
 * \file utility_2.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 几何接口
 * \version 0.1
 * \date 2023-06-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GEOMERY_UTILITY_H
#define GEOMERY_UTILITY_H

#include "string_2.h"
#include "tf_2.h"
#include "buffer_2.h"
#include "bool_2.h"

class Utility_2 {
  public:
  /**
   * \brief 合并垂直方向上的两条线段
   *
   * \param seg1
   * \param seg2
   * \return Segment
   */
  static Segment MergeVerticalSegments(const Segment &seg1, const Segment &seg2);

  /**
   * \brief 比较点大小
   *
   * \param pt1
   * \param pt2
   * \return true
   * \return false
   */
  static bool CompPoint(const Point &pt1, const Point &pt2);

  /**
   * \brief 比较线段大小
   *
   * \param seg1
   * \param seg2
   * \return true
   * \return false
   */
  static bool CompSegment(const Segment &seg1, const Segment &seg2);

  /**
   * \brief 设置polygon
   *
   * \return Polygon
   */
  static Polygon SetData(const std::vector<Point> &data);

  /**
   * \brief double保留小数
   * 
   * \param dval 
   * \param n 
   * \return double 
   */
  static double DRound(double dval, int n);
};

#endif // GEOMERY_UTILITY_H
