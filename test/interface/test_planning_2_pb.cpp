/**
 * \file test_planning_2_pb.cpp
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief
 * \version 0.1
 * \date 2023-06-15
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "interface/planning_2.h"
#include "visualization/vis_2.h"

#include <cstdlib>
#include <fstream>

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    std::cout << "usage: ./test_interface_planning_2_pb xxx.data" << std::endl;
    return 1;
  }

  std::string   fn = argv[1];
  std::ifstream alt_cin;
  alt_cin.open(fn.c_str(), std::ios::in | std::ios::binary);
  std::stringstream sstr;
  sstr << alt_cin.rdbuf();
  protocol::apollo::planning::V1::PathPlanParam *path_plan_param = new protocol::apollo::planning::V1::PathPlanParam();
  path_plan_param->ParseFromString(sstr.str());

  double wp_heading_pb, wp_interval_pb, wp_offset_pb;
  wp_heading_pb   = path_plan_param->wp_heading();
  wp_interval_pb  = path_plan_param->wp_interval();
  wp_offset_pb    = path_plan_param->wp_offset();
  bool wp_preview = path_plan_param->wp_preview();

  protocol::apollo::planning::V1::Point wp_start_pb = path_plan_param->wp_start();
  Point                                 wp_start(wp_start_pb.x(), wp_start_pb.y());

  protocol::apollo::planning::V1::Borders work_area_pb = path_plan_param->work_area();
  protocol::apollo::planning::V1::Polygon outline_pb   = work_area_pb.borders();

  std::vector<Point> outline;
  for (int i = 0; i < outline_pb.vertex_size(); i++)
  {
    protocol::apollo::planning::V1::Point vertex = outline_pb.vertex(i);
    Point                                 pt(vertex.x(), vertex.y());
    outline.push_back(pt);
  }

  std::vector<std::vector<Point>> holes;
  for (int j = 0; j < work_area_pb.obstacle_polygons_size(); j++)
  {
    std::vector<Point>                      hole;
    protocol::apollo::planning::V1::Polygon hole_pb = work_area_pb.obstacle_polygons(j);

    for (int k = 0; k < hole_pb.vertex_size(); k++)
    {
      protocol::apollo::planning::V1::Point vertex = hole_pb.vertex(k);
      Point                                 pt_hole(vertex.x(), vertex.y());
      hole.push_back(pt_hole);
    }
    holes.push_back(hole);
  }

  std::string ret = Plan_2(sstr.str());
  protocol::apollo::planning::V1::RoutePointsPlanned *route_planned =
      new protocol::apollo::planning::V1::RoutePointsPlanned();
  route_planned->ParseFromString(ret);

  std::vector<Point>        path;
  std::vector<WaypointType> types;
  std::vector<int>          line_nums;
  for (int i = 0; i < route_planned->wps_size(); i++)
  {
    protocol::apollo::planning::V1::RoutePoint wps = route_planned->wps(i);
    Point                                      p(wps.p().x(), wps.p().y());
    path.push_back(p);
    line_nums.push_back(wps.route_id());

    if (wps.route_type() == protocol::apollo::planning::V1::RoutePoint::NORMAL_START)
    {
      types.push_back(WaypointType::NORMAL_START);
    } else if (wps.route_type() == protocol::apollo::planning::V1::RoutePoint::NORMAL_END)
    {
      types.push_back(WaypointType::NORMAL_END);
    } else if (wps.route_type() == protocol::apollo::planning::V1::RoutePoint::TRANSITION)
    {
      types.push_back(WaypointType::TRANSITION);
    } else
    {
      types.push_back(WaypointType::DETOUR);
    }
  }
  std::string file_path = std::string(getenv("HOME")) + "/test_planning";
  Vis_2       vis(file_path);
  vis.Clear();

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
  if (!path.empty())
  {
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
  }

  vis.NewFile("normal-track", path1_segs, "green");
  vis.NewFile("other-track", path2_segs, "yellow");

  vis.NewFile("normal-pts", normal, "green");
  vis.NewFile("transition-pts", transition, "pink");
  vis.NewFile("detour-pts", detour, "red");

  vis.NewFile("start-pt", {Point(wp_start.x() - origin_pt.x(), wp_start.y() - origin_pt.y())}, "yellow");

  if (path.size() > 2)
  {
    Point path_start(path.front().x() - origin_pt.x(), path.front().y() - origin_pt.y());
    Point path_end(path.back().x() - origin_pt.x(), path.back().y() - origin_pt.y());
    vis.NewFile("path-start", {path_start}, "red");
    vis.NewFile("path-end", {path_end}, "green");
  }

  vis.Show();

  return 0;
}