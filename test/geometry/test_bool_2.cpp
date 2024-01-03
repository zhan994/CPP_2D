/**
 * \file test_bool_2.cpp
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 几何布尔运算测试
 * \version 0.1
 * \date 2023-06-12
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "geometry/utility_2.h"
#include "visualization/vis_2.h"

#include <iostream>

int main()
{
  std::shared_ptr<Vis_2> vis_ptr = std::make_shared<Vis_2>();

  std::cout << " ================== 1. Intersection ================== " << std::endl;
  Segment seg1(Point(1, 2.2), Point(2, 3.3));
  Segment seg2(Point(1, 3.3), Point(2, 2.2));
  Segment seg3(Point(1, 3.3), Point(-2, -2.2));
  Segment seg4(Point(1, 1), Point(2, 2));
  Segment seg5(Point(1.5, 1.5), Point(2.5, 2.5));
  std::cout << String_2::SegmentToString(seg1) << " & " << String_2::SegmentToString(seg2)
            << "intersection state: " << bg::intersects(seg1, seg2) << std::endl;
  if (bg::intersects(seg1, seg2))
  {
    std::vector<Point> res;
    Bool_2::Intersection(seg1, seg2, res);
    for (const auto &pt : res) std::cout << "Intersection: " << String_2::PointToString(pt) << std::endl;
  }

  std::cout << String_2::SegmentToString(seg2) << " & " << String_2::SegmentToString(seg3)
            << "intersection state: " << bg::intersects(seg2, seg3) << std::endl;
  if (bg::intersects(seg2, seg3))
  {
    std::vector<Point> res;
    Bool_2::Intersection(seg2, seg3, res);
    for (const auto &pt : res) std::cout << "Intersection: " << String_2::PointToString(pt) << std::endl;
  }
  std::cout << String_2::SegmentToString(seg4) << " & " << String_2::SegmentToString(seg5)
            << "intersection state: " << bg::intersects(seg4, seg5) << std::endl;
  if (bg::intersects(seg4, seg5))
  {
    std::vector<Point> res;
    Bool_2::Intersection(seg4, seg5, res);
    for (const auto &pt : res) std::cout << "Intersection: " << String_2::PointToString(pt) << std::endl;
  }

  Polygon pg1;
  pg1.outer().push_back(Point(0, 0));
  pg1.outer().push_back(Point(1, 0));
  pg1.outer().push_back(Point(1, 1));
  pg1.outer().push_back(Point(0, 1));
  pg1.outer().push_back(Point(0, 0));
  bg::correct(pg1);
  std::cout << "pg1 edges num = " << pg1.outer().size() << std::endl;

  Segment seg6(Point(-1, -1), Point(2, 2));
  std::cout << String_2::SegmentToString(seg6) << " & pg1 intersection state: " << bg::intersects(seg6, pg1)
            << std::endl;
  if (bg::intersects(seg6, pg1))
  {
    std::vector<Point> res;
    Bool_2::Intersection(seg6, pg1, res);
    for (const auto &pt : res) std::cout << "Intersection: " << String_2::PointToString(pt) << std::endl;
  }

  Segment seg7(Point(0, 0.5), Point(0, 1));
  std::cout << String_2::SegmentToString(seg7) << " & pg1 intersection state: " << bg::intersects(seg7, pg1)
            << std::endl;
  if (bg::intersects(seg7, pg1))
  {
    std::vector<Point> res;
    Bool_2::Intersection(seg7, pg1, res);
    for (const auto &pt : res) std::cout << "Intersection: " << String_2::PointToString(pt) << std::endl;
  }

  Segment seg8(Point(-1, 1), Point(1, -1));
  std::cout << String_2::SegmentToString(seg8) << " & pg1 intersection state: " << bg::intersects(seg8, pg1)
            << std::endl;
  if (bg::intersects(seg8, pg1))
  {
    std::vector<Point> res;
    Bool_2::Intersection(seg8, pg1, res);
    for (const auto &pt : res) std::cout << "Intersection: " << String_2::PointToString(pt) << std::endl;
  }

  Polygon pg2;
  pg2.outer().push_back(Point(-10, -10));
  pg2.outer().push_back(Point(10, -10));
  pg2.outer().push_back(Point(10, 10));
  pg2.outer().push_back(Point(-10, 10));
  pg2.outer().push_back(Point(-10, -10));
  pg2.inners().push_back(pg1.outer());
  bg::correct(pg2);
  Segment seg9(Point(-9, -9), Point(11, 11));
  std::cout << String_2::SegmentToString(seg9) << " & pg2 intersection state: " << bg::intersects(seg9, pg2)
            << std::endl;
  if (bg::intersects(seg9, pg2))
  {
    std::vector<Point> res;
    Bool_2::Intersection(seg9, pg2, res);
    for (const auto &pt : res) std::cout << "Intersection: " << String_2::PointToString(pt) << std::endl;
  }
  if (0)
  {
    vis_ptr->Clear();
    vis_ptr->NewFile("pgs", {pg2});
    vis_ptr->NewFile("segs", {seg9});
    vis_ptr->Show();

    return 1;
  }

  Polygon pg3;
  pg3.outer().push_back(Point(-10, -10));
  pg3.outer().push_back(Point(10, -10));
  pg3.outer().push_back(Point(10, 10));
  pg3.outer().push_back(Point(-10, 10));
  pg3.outer().push_back(Point(-10, -10));
  bg::correct(pg3);
  if (bg::intersects(pg1, pg3))
  {
    std::vector<Polygon> res;
    Bool_2::Intersection(pg1, pg3, res);
    std::cout << "pg1 && pg3 = " << bg::intersects(pg1, pg3) << " , intersection polygon num = " << res.size()
              << std::endl;
  }

  std::cout << " ================== 2. Difference ================== " << std::endl;
  std::vector<Polygon> pg3_diff_pg1;
  Bool_2::Difference(pg3, pg1, pg3_diff_pg1);
  std::cout << pg3_diff_pg1.front().outer().size() << std::endl;
  if (0)
  {
    vis_ptr->Clear();
    // vis_ptr->NewFile("pg3", {pg3});
    // vis_ptr->NewFile("pg1", {pg1});
    vis_ptr->NewFile("diff", pg3_diff_pg1);
    vis_ptr->Show();

    return 1;
  }

  std::cout << " ================== 3. Union ================== " << std::endl;
  std::vector<Polygon> pg3_union_pg1;
  Bool_2::Union(pg3, pg1, pg3_union_pg1);
  std::cout << pg3_union_pg1.size() << std::endl;
  std::cout << pg3_union_pg1.front().outer().size() << std::endl;
  if (0)
  {
    vis_ptr->Clear();
    // vis_ptr->NewFile("pg3", {pg3});
    // vis_ptr->NewFile("pg1", {pg1});
    vis_ptr->NewFile("union", pg3_union_pg1);
    vis_ptr->Show();

    return 1;
  }

  std::cout << " ================== 4. CoveredBy ================== " << std::endl;
  Segment seg10(Point(-10, -10), Point(10, 10));
  Segment seg11(Point(-9, -9), Point(0, 4));
  Segment seg12(Point(-10, -10), Point(10, -10));
  Polygon pg4;
  pg4.outer().push_back(Point(-10, -10));
  pg4.outer().push_back(Point(10, -10));
  pg4.outer().push_back(Point(0, 5));
  pg4.outer().push_back(Point(10, 10));
  pg4.outer().push_back(Point(-10, 10));
  pg4.outer().push_back(Point(-10, -10));
  bg::correct(pg4);
  std::cout << Bool_2::CoveredBy(seg10, pg4) << std::endl;
  std::cout << Bool_2::CoveredBy(seg11, pg4) << std::endl;
  std::cout << Bool_2::CoveredBy(seg12, pg4) << std::endl;
  if (1)
  {
    vis_ptr->Clear();
    vis_ptr->NewFile("seg", {seg10});
    vis_ptr->NewFile("seg1", {seg11});
    vis_ptr->NewFile("pg", {pg4});
    vis_ptr->Show();

    return 1;
  }

  return 0;
}
