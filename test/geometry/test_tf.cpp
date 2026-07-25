/**
 * \file test_tf.cpp
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 几何坐标变换
 * \version 0.1
 * \date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "geometry/utility.h"
#include <iostream>

int main()
{
  std::cout << " ================== 1. Rotation ================== " << std::endl;
  Point pt_before(0, 0);
  Point center(1, 1);
  Point pt_after = TF::Rotate(pt_before, center, 45.0);
  std::cout << String::PointToString(pt_before) << " rotated 45deg according to " << String::PointToString(center)
            << " = " << String::PointToString(pt_after) << std::endl;

  Segment seg_test(Point(-1, -1), Point(1, 1));
  Segment seg_test_1 = TF::Rotate(seg_test, Point(0, 0), 45);
  std::cout << String::SegmentToString(seg_test) << " rotated 45deg according to " << String::PointToString(Point(0, 0))
            << " = " << String::SegmentToString(seg_test_1) << std::endl;

  return 0;
}