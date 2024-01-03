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

  auto route_ptr = Set_2(outline, holes, wp_start, wp_heading_pb, wp_interval_pb, wp_offset_pb, wp_preview);
  std::vector<Point>        paths;
  std::vector<WaypointType> types;
  std::vector<int>          line_nums;
  Plan_2(route_ptr, paths, types, line_nums);

  return 0;
}