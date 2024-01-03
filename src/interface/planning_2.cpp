#include "interface/planning_2.h"

std::shared_ptr<Route_2> Set_2(const std::vector<Point> &             outline,
                               const std::vector<std::vector<Point>> &holes,
                               const Point &                          start_pt,
                               double                                 rotation,
                               double                                 interval,
                               double                                 offset,
                               bool                                   preview)
{
  auto route_ptr = std::make_shared<Route_2>();
  auto t1        = std::chrono::steady_clock::now();

  std::vector<std::vector<Point>> holes_used;
  if (!preview)
    holes_used = holes;
  route_ptr->SetWork(outline, holes_used);

  auto t2 = std::chrono::steady_clock::now();
  route_ptr->SetParam(start_pt, rotation, interval, offset, preview);
  auto                          t3       = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed1 = t2 - t1;
  std::chrono::duration<double> elapsed2 = t3 - t2;
  std::cout << " ***************** Set Work Polygon: " << 1000 * elapsed1.count() << " ms ***************** "
            << std::endl;
  std::cout << " ***************** Set Param: " << 1000 * elapsed2.count() << " ms ***************** " << std::endl;

  return route_ptr;
}

std::shared_ptr<Route_2> Set_2(const void *data, int size)
{
  protocol::apollo::planning::V1::PathPlanParam path_plan_param;
  path_plan_param.ParseFromArray(data, size);

  double wp_heading_pb, wp_interval_pb, wp_offset_pb;
  wp_heading_pb   = path_plan_param.wp_heading();
  wp_interval_pb  = path_plan_param.wp_interval();
  wp_offset_pb    = path_plan_param.wp_offset();
  bool wp_preview = path_plan_param.wp_preview();

  protocol::apollo::planning::V1::Point   wp_start_pb  = path_plan_param.wp_start();
  protocol::apollo::planning::V1::Borders work_area_pb = path_plan_param.work_area();
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

  Point wp_start(wp_start_pb.x(), wp_start_pb.y());
  return Set_2(outline, holes, wp_start, wp_heading_pb, wp_interval_pb, wp_offset_pb, wp_preview);
}

void Plan_2(std::shared_ptr<Route_2>   route_ptr,
            std::vector<Point> &       path,
            std::vector<WaypointType> &types,
            std::vector<int> &         line_nums)
{
  auto t1 = std::chrono::steady_clock::now();
  route_ptr->PlanWithRotation(path, types, line_nums);
  auto                          t2       = std::chrono::steady_clock::now();
  std::chrono::duration<double> elapsed1 = t2 - t1;
  std::cout << " ***************** Plan: " << 1000 * elapsed1.count() << " ms ***************** " << std::endl;
}

protocol::apollo::planning::V1::RoutePointsPlanned Plan_2(std::shared_ptr<Route_2> route_ptr)
{
  std::vector<Point>        paths;
  std::vector<WaypointType> types;
  std::vector<int>          line_nums;
  Plan_2(route_ptr, paths, types, line_nums);

  protocol::apollo::planning::V1::RoutePointsPlanned route_points_planned;
  for (int m = 0; m < paths.size(); m++)
  {
    protocol::apollo::planning::V1::RoutePoint *wp = route_points_planned.add_wps();
    wp->set_route_id(line_nums[m]);
    protocol::apollo::planning::V1::Point *p = new protocol::apollo::planning::V1::Point();
    p->set_x(paths[m].x());
    p->set_y(paths[m].y());
    wp->set_allocated_p(p);

    if (types[m] == WaypointType::NORMAL_START)
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::NORMAL_START);
    } else if (types[m] == WaypointType::NORMAL_END)
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::NORMAL_END);
    } else if (types[m] == WaypointType::TRANSITION)
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::TRANSITION);
    } else
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::DETOUR);
    }
  }

  return route_points_planned;
}

void Stop_2(std::shared_ptr<Route_2> route_ptr)
{
  route_ptr->Stop();
}

void Plan_2(const std::vector<Point> &             outline,
            const std::vector<std::vector<Point>> &holes,
            const Point &                          start_pt,
            double                                 rotation,
            double                                 interval,
            double                                 offset,
            bool                                   preview,
            std::vector<Point> &                   path,
            std::vector<WaypointType> &            types,
            std::vector<int> &                     line_nums)
{
  Route_2 route;
  auto    t1 = std::chrono::steady_clock::now();

  std::vector<std::vector<Point>> holes_used;
  if (!preview)
    holes_used = holes;
  route.SetWork(outline, holes_used);

  auto t2 = std::chrono::steady_clock::now();
  route.SetParam(start_pt, rotation, interval, offset, preview);
  auto t3 = std::chrono::steady_clock::now();
  route.PlanWithRotation(path, types, line_nums);
  auto t4 = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed1 = t2 - t1;
  std::chrono::duration<double> elapsed2 = t3 - t2;
  std::chrono::duration<double> elapsed3 = t4 - t3;
  std::chrono::duration<double> elapsed4 = t4 - t1;
  std::cout << " ================= Over ================= " << std::endl;
  std::cout << " ***************** Set Work Polygon: " << 1000 * elapsed1.count() << " ms ***************** "
            << std::endl;
  std::cout << " ***************** Set Param: " << 1000 * elapsed2.count() << " ms ***************** " << std::endl;
  std::cout << " ***************** Plan: " << 1000 * elapsed3.count() << " ms ***************** " << std::endl;
  std::cout << " ***************** Total cost: " << 1000 * elapsed4.count() << " ms ***************** " << std::endl;
}

std::string Plan_2(const std::string &param)
{
  // note: <fix> memory bug, avoid pointer which 'new' memory inside interface
  protocol::apollo::planning::V1::PathPlanParam path_plan_param;
  path_plan_param.ParseFromString(param);

  double wp_heading_pb, wp_interval_pb, wp_offset_pb;
  wp_heading_pb   = path_plan_param.wp_heading();
  wp_interval_pb  = path_plan_param.wp_interval();
  wp_offset_pb    = path_plan_param.wp_offset();
  bool wp_preview = path_plan_param.wp_preview();

  protocol::apollo::planning::V1::Point   wp_start_pb  = path_plan_param.wp_start();
  protocol::apollo::planning::V1::Borders work_area_pb = path_plan_param.work_area();
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

  std::vector<Point>        paths;
  std::vector<WaypointType> types;
  std::vector<int>          line_nums;
  Point                     wp_start(wp_start_pb.x(), wp_start_pb.y());

  Plan_2(outline, holes, wp_start, wp_heading_pb, wp_interval_pb, wp_offset_pb, wp_preview, paths, types, line_nums);

  protocol::apollo::planning::V1::RoutePointsPlanned route_points_planned;
  for (int m = 0; m < paths.size(); m++)
  {
    protocol::apollo::planning::V1::RoutePoint *wp = route_points_planned.add_wps();
    wp->set_route_id(line_nums[m]);
    protocol::apollo::planning::V1::Point *p = new protocol::apollo::planning::V1::Point();
    p->set_x(paths[m].x());
    p->set_y(paths[m].y());
    wp->set_allocated_p(p);

    if (types[m] == WaypointType::NORMAL_START)
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::NORMAL_START);
    } else if (types[m] == WaypointType::NORMAL_END)
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::NORMAL_END);
    } else if (types[m] == WaypointType::TRANSITION)
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::TRANSITION);
    } else
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::DETOUR);
    }
  }

  std::string str_serialize;
  bool        b_serialize = route_points_planned.SerializeToString(&str_serialize);
  if (b_serialize)
  {
    return str_serialize;
  } else
  {
    return "";
  }
}

protocol::apollo::planning::V1::RoutePointsPlanned Plan_2(const void *data, int size)
{
  protocol::apollo::planning::V1::PathPlanParam path_plan_param;
  path_plan_param.ParseFromArray(data, size);

  double wp_heading_pb, wp_interval_pb, wp_offset_pb;
  wp_heading_pb   = path_plan_param.wp_heading();
  wp_interval_pb  = path_plan_param.wp_interval();
  wp_offset_pb    = path_plan_param.wp_offset();
  bool wp_preview = path_plan_param.wp_preview();

  protocol::apollo::planning::V1::Point   wp_start_pb  = path_plan_param.wp_start();
  protocol::apollo::planning::V1::Borders work_area_pb = path_plan_param.work_area();
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

  std::vector<Point>        paths;
  std::vector<WaypointType> types;
  std::vector<int>          line_nums;
  Point                     wp_start(wp_start_pb.x(), wp_start_pb.y());

  Plan_2(outline, holes, wp_start, wp_heading_pb, wp_interval_pb, wp_offset_pb, wp_preview, paths, types, line_nums);

  protocol::apollo::planning::V1::RoutePointsPlanned route_points_planned;
  for (int m = 0; m < paths.size(); m++)
  {
    protocol::apollo::planning::V1::RoutePoint *wp = route_points_planned.add_wps();
    wp->set_route_id(line_nums[m]);
    protocol::apollo::planning::V1::Point *p = new protocol::apollo::planning::V1::Point();
    p->set_x(paths[m].x());
    p->set_y(paths[m].y());
    wp->set_allocated_p(p);

    if (types[m] == WaypointType::NORMAL_START)
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::NORMAL_START);
    } else if (types[m] == WaypointType::NORMAL_END)
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::NORMAL_END);
    } else if (types[m] == WaypointType::TRANSITION)
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::TRANSITION);
    } else
    {
      wp->set_route_type(protocol::apollo::planning::V1::RoutePoint::DETOUR);
    }
  }

  return route_points_planned;
}
