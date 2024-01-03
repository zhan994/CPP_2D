/**
 * \file test_tf_2.cpp
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 几何坐标变换
 * \version 0.1
 * \date 2023-06-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "geometry/utility_2.h"
#include <iostream>

int main()
{
  std::cout << " ================== 1. Rotation ================== " << std::endl;
  Point pt_before(0, 0);
  Point center(1, 1);
  Point pt_after = TF_2::Rotate(pt_before, center, 45.0);
  std::cout << String_2::PointToString(pt_before) << " rotated 45deg according to " << String_2::PointToString(center)
            << " = " << String_2::PointToString(pt_after) << std::endl;

  Segment seg_test(Point(-1, -1), Point(1, 1));
  Segment seg_test_1 = TF_2::Rotate(seg_test, Point(0, 0), 45);
  std::cout << String_2::SegmentToString(seg_test) << " rotated 45deg according to "
            << String_2::PointToString(Point(0, 0)) << " = " << String_2::SegmentToString(seg_test_1) << std::endl;

  return 0;
}