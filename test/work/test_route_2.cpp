/**
 * \file test_route_2.cpp
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 航线规划相关功能测试
 * \version 0.1
 * \date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <iostream>
#include "work/route_2.h"
#include "visualization/vis_2.h"

int main()
{
  Vis_2 vis;
  vis.Clear();

  // std::vector<Point>              outline = {Point(0., 0.),  Point(10., 0.),  Point(10., 5.), Point(8., 7.),
  //                               Point(10., 8.), Point(10., 10.), Point(5., 13.), Point(0., 10.)};
  // std::vector<std::vector<Point>> holes   = {{Point(2., 4.), Point(2., 6.), Point(8., 6.), Point(8., 4.)},
  //                                          {Point(4., -1.), Point(2.5, 2.), Point(4., 3.), Point(5.5, 2.)}};
  // Point                           start_pt(8.0, 6.0);

  std::vector<Point>              outline = {Point(13220710.834, 3760418.741), Point(13220806.594, 3760418.79),
                                Point(13220801.438, 3760359.008), Point(13220702.703, 3760360.44)};
  std::vector<std::vector<Point>> holes   = {{Point(13220779.412, 3760402.492), Point(13220786.001, 3760359.443),
                                            Point(13220781.39, 3760356.619), Point(13220760.697, 3760371.512),
                                            Point(13220760.227, 3760373.227), Point(13220773.367, 3760403.283)}};
  Point                           start_pt(13220709.655, 3760420.094);

  double rotation = 0.;
  double interval = 2;
  double offset   = 0.3;
  bool   preview  = false;

  Route_2 route;
  route.SetWork(outline, holes);
  route.SetParam(start_pt, rotation, interval, offset, preview);

  std::vector<Point>        path;
  std::vector<WaypointType> types;
  std::vector<int>          line_nums;
  route.PlanWithRotation(path, types, line_nums);

  // vis
  Point   origin_pt = outline[0];
  Polygon outline_pg;
  for (const auto &pt : outline) outline_pg.outer().push_back(Point(pt.x() - origin_pt.x(), pt.y() - origin_pt.y()));
  outline_pg.outer().push_back(Point(0, 0));
  vis.NewFile("outline", outline_pg, "black");

  std::vector<Polygon> holes_pg;
  for (const auto &hole : holes)
  {
    Polygon hole_pg;
    for (const auto &pt : hole) hole_pg.outer().push_back(Point(pt.x() - origin_pt.x(), pt.y() - origin_pt.y()));
    hole_pg.outer().push_back(Point(hole[0].x() - origin_pt.x(), hole[0].y() - origin_pt.y()));

    holes_pg.push_back(hole_pg);
  }
  vis.NewFile("holes", holes_pg, "red");

  std::vector<Segment> path1_segs, path2_segs;
  std::vector<Point>   normal, transition, detour;
  for (size_t i = 0; i < path.size() - 1; ++i)
  {
    Point   pt1(path[i].x() - origin_pt.x(), path[i].y() - origin_pt.y());
    Point   pt2(path[i + 1].x() - origin_pt.x(), path[i + 1].y() - origin_pt.y());
    Segment seg(pt1, pt2);
    if (types[i] == WaypointType::NORMAL_START)
    {
      path1_segs.push_back(seg);
      normal.push_back(pt1);
    } else
    {
      path2_segs.push_back(seg);
      if (types[i] == WaypointType::NORMAL_END)
      {
        normal.push_back(pt1);
      } else if (types[i] == WaypointType::TRANSITION)
      {
        transition.push_back(pt1);
      } else
      {
        detour.push_back(pt1);
      }
    }
  }
  vis.NewFile("normal-track", path1_segs, "green");
  vis.NewFile("other-track", path2_segs, "yellow");

  vis.NewFile("normal-pts", normal, "green");
  vis.NewFile("transition-pts", transition, "pink");
  vis.NewFile("detour-pts", detour, "red");

  Point path_start(path.front().x() - origin_pt.x(), path.front().y() - origin_pt.y());
  Point path_end(path.back().x() - origin_pt.x(), path.back().y() - origin_pt.y());
  vis.NewFile("start-pt", {Point(start_pt.x() - origin_pt.x(), start_pt.y() - origin_pt.y())}, "yellow");
  vis.NewFile("path-start", {path_start}, "red");
  vis.NewFile("path-end", {path_end}, "green");

  vis.Show();

  return 0;
}