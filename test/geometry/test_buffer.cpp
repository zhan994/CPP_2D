/**
 * \file test_buffer.cpp
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 几何外扩收缩以及bbox等测试
 * \version 0.1
 * \date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "geometry/utility.h"
#include "visualization/vis.h"
#include <iostream>

int main()
{
  std::shared_ptr<Vis> vis_ptr = std::make_shared<Vis>();

  std::cout << " ================== 1. Shrink ================== " << std::endl;
  Point pt_before(0, 0);
  Point center(1, 1);

  Polygon pg1;
  pg1.outer().push_back(Point(0, 0));
  pg1.outer().push_back(Point(1, 0));
  pg1.outer().push_back(Point(1, 1));
  pg1.outer().push_back(Point(0, 1));
  pg1.outer().push_back(Point(0, 0));
  bg::correct(pg1);

  Polygon pg2;
  pg2.outer().push_back(Point(0, 0));
  pg2.outer().push_back(Point(1, 0));
  pg2.outer().push_back(Point(1, 1));
  pg2.outer().push_back(Point(0.5, 0.5));
  pg2.outer().push_back(Point(0, 1));
  pg2.outer().push_back(Point(0, 0));
  bg::correct(pg2);

  Segment seg_before_shrink(pt_before, center);
  Segment seg_after_shrink = Buffer::Shrink(seg_before_shrink);
  std::cout << String::SegmentToString(seg_before_shrink) << " -> " << String::SegmentToString(seg_after_shrink)
            << std::endl;

  Segment seg_before_shrink1(center, pt_before);
  Segment seg_after_shrink1 = Buffer::Shrink(seg_before_shrink1);
  std::cout << String::SegmentToString(seg_before_shrink1) << " -> " << String::SegmentToString(seg_after_shrink1)
            << std::endl;

  Segment seg_before_shrink2(pt_before, Point(0, 2));
  Segment seg_after_shrink2 = Buffer::Shrink(seg_before_shrink2);
  std::cout << String::SegmentToString(seg_before_shrink2) << " -> " << String::SegmentToString(seg_after_shrink2)
            << std::endl;

  Segment seg_before_shrink3(pt_before, Point(2, 0));
  Segment seg_after_shrink3 = Buffer::Shrink(seg_before_shrink3);
  std::cout << String::SegmentToString(seg_before_shrink3) << " -> " << String::SegmentToString(seg_after_shrink3)
            << std::endl;

  Polygon pg1_after_shrink;
  Buffer::Shrink(pg1, pg1_after_shrink);
  if (0)
  {
    vis_ptr->Clear();
    vis_ptr->NewFile("before", {pg1});
    vis_ptr->NewFile("after", {pg1_after_shrink});
    vis_ptr->Show();

    return 1;
  }

  Polygon pg2_after_shrink;
  Buffer::Shrink(pg2, pg2_after_shrink);
  if (1)
  {
    vis_ptr->Clear();
    vis_ptr->NewFile("before", {pg2});
    vis_ptr->NewFile("after", {pg2_after_shrink});
    vis_ptr->Show();

    return 1;
  }

  return 0;
}
