/**
 * \file test_utility_2.cpp
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 几何工具函数测试
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
  std::cout << " ================== 1. MergeVerSeg ================== " << std::endl;
  Segment seg1(Point(0, -1), Point(0, 2));
  Segment seg2(Point(0, 0.1), Point(0, 4.5));
  Segment mv_seg = Utility_2::MergeVerticalSegments(seg1, seg2);
  std::cout << String_2::SegmentToString(seg1) << " mv " << String_2::SegmentToString(seg2) << " = "
            << String_2::SegmentToString(mv_seg) << std::endl;

  std::cout << " ================== 2. Sort Seg ================== " << std::endl;
  std::vector<Segment> segs = {seg1, seg2, mv_seg};
  std::sort(segs.begin(), segs.end(), Utility_2::CompSegment);
  for (const auto &seg : segs) std::cout << String_2::SegmentToString(seg) << std::endl;

  std::cout << " ================== 3. bg::equal ================== " << std::endl;
  std::cout << String_2::SegmentToString(seg1) << " == " << String_2::SegmentToString(seg2) << " : "
            << bg::equals(seg1, seg2) << std::endl;

  std::cout << String_2::SegmentToString(seg1) << " == " << String_2::SegmentToString(seg1) << " : "
            << bg::equals(seg1, seg1) << std::endl;

  Segment seg3(Point(0, -1), Point(0, 1.999999));
  std::cout << String_2::SegmentToString(seg1) << " == " << String_2::SegmentToString(seg3) << " : "
            << bg::equals(seg1, seg3) << std::endl;

  // std::cout << String_2::SegmentToString(seg1) << " == " << String_2::SegmentToString(seg1) << " : "
  //           << bg::equals(seg1, seg1) << std::endl;

  return 0;
}