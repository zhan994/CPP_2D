/**
 * \file test_work_2.cpp
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 2d 工作区相关功能测试
 * \version 0.1
 * \date 2023-06-12
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "work/work_2.h"
#include "visualization/vis_2.h"

int main()
{
  std::cout << " ================== 1. Work ================== " << std::endl;
  std::shared_ptr<Vis_2> vis_ptr = std::make_shared<Vis_2>();

  Work_2                          work;
  // std::vector<Point>              outline = {Point(0., 0.),  Point(10., 0.),  Point(10., 5.), Point(8., 7.),
  //                               Point(10., 8.), Point(10., 10.), Point(5., 13.), Point(0., 10.)};
  // std::vector<std::vector<Point>> holes   = {{Point(2., 4.), Point(2., 6.), Point(8., 6.), Point(8., 4.)},
  //                                          {Point(4., -1.), Point(2.5, 2.), Point(4., 3.), Point(5.5, 2.)}};
  // Point                           start_pt(8.0, 6.0);

  // std::vector<Point>              outline = {Point(13220710.834, 3760418.741), Point(13220806.594, 3760418.79),
  //                               Point(13220801.438, 3760359.008), Point(13220702.703, 3760360.44)};
  // std::vector<std::vector<Point>> holes   = {{Point(13220779.412, 3760402.492), Point(13220786.001, 3760359.443),
  //                                           Point(13220781.39, 3760356.619), Point(13220760.697, 3760371.512),
  //                                           Point(13220760.227, 3760373.227), Point(13220773.367, 3760403.283)}};
  // Point                           start_pt(13220709.655, 3760420.094);

  std::vector<Point> outline = {
      Point(13389094.140655287, 3877527.7209785804), Point(13388713.822361792, 3877503.039012792),
      Point(13388712.884617073, 3877502.578079747),  Point(13388712.604625564, 3877502.260065101),
      Point(13388712.266173882, 3877501.2714929637), Point(13388712.757877694, 3877493.374971112),
      Point(13388650.676113691, 3877489.6474841135), Point(13388650.006104274, 3877495.8616606235),
      Point(13388645.665685736, 3877536.0558904116), Point(13388646.16627359, 3877599.8214993235),
      Point(13388649.952735327, 3877602.296858313),  Point(13388650.369866425, 3877602.7260328643),
      Point(13388650.503920766, 3877602.9444162524), Point(13388650.697807001, 3877603.510629358),
      Point(13388651.640803542, 3877612.1051631677), Point(13388653.69418473, 3877616.3833663366),
      Point(13388656.710663125, 3877618.9419734776), Point(13388669.48597238, 3877621.941461509),
      Point(13388797.642382689, 3877632.965550693),  Point(13388822.085909575, 3877633.90943736),
      Point(13388822.125353439, 3877633.9115314116), Point(13388822.142644154, 3877633.912700023),
      Point(13388822.182011072, 3877633.9159324816), Point(13388998.79942721, 3877650.98962922),
      Point(13389117.615124218, 3877647.813860079),  Point(13389125.682685317, 3877646.76221793),
      Point(13389142.420678213, 3877532.1410794375), Point(13389132.710383823, 3877531.5356941433),
      Point(13389121.919103475, 3877534.057680025),  Point(13389121.679830903, 3877534.0916671036),
      Point(13389121.574347517, 3877534.097222098),  Point(13389121.33282866, 3877534.0885545877),
      Point(13389109.670256672, 3877532.630698699),  Point(13389109.546607608, 3877532.6094415863),
      Point(13389109.492893208, 3877532.5976473307), Point(13389109.371712983, 3877532.5651464346)};

  std::vector<std::vector<Point>> holes = {
      {Point(13389118.014790455, 3877573.78118593), Point(13389111.289844487, 3877573.502006333),
       Point(13389109.124279905, 3877575.747490848), Point(13389109.639825296, 3877582.3672849997),
       Point(13389110.349045616, 3877583.249948572), Point(13389116.336672481, 3877585.0679193474),
       Point(13389119.17216342, 3877583.1801269487), Point(13389119.905433921, 3877575.9629819626)},
      {Point(13389092.535134014, 3877653.8248568806), Point(13389091.890093848, 3877646.7586354264),
       Point(13389090.23598536, 3877645.234701561), Point(13389063.81369735, 3877644.992739897),
       Point(13389062.518941423, 3877645.976456408), Point(13389060.432288475, 3877653.7518010456),
       Point(13389062.760360235, 3877656.708254831), Point(13389090.60091198, 3877655.9986731396)},
      {Point(13389059.086253073, 3877651.66672246), Point(13389059.179312203, 3877636.2022982),
       Point(13389054.322126701, 3877634.009236709), Point(13389040.087246217, 3877646.712389751),
       Point(13389041.63036579, 3877651.7306167274), Point(13389056.71387812, 3877653.732461231)},
      {Point(13389045.876729455, 3877647.960815455), Point(13389046.732259564, 3877655.231134435),
       Point(13389044.678629782, 3877657.5492951204), Point(13388988.283437848, 3877657.759884335),
       Point(13388986.088789439, 3877655.0636636675), Point(13388987.51640522, 3877648.303583091),
       Point(13388988.875295388, 3877647.1873949473), Point(13389044.199691879, 3877646.488415156)},
      {Point(13388980.438466249, 3877653.195992675), Point(13388979.801396549, 3877643.779162275),
       Point(13388978.322217021, 3877642.295353771), Point(13388857.88497692, 3877633.769391187),
       Point(13388856.60482106, 3877634.6010319707), Point(13388854.493896144, 3877640.42225769),
       Point(13388856.423349807, 3877643.4844026295), Point(13388978.068906346, 3877655.4935467127)},
      {Point(13388824.862375138, 3877630.154592941), Point(13388831.947598401, 3877631.1475435663),
       Point(13388833.757366786, 3877629.94450769), Point(13388835.558747606, 3877623.123997898),
       Point(13388834.266551992, 3877620.8701382717), Point(13388827.115311002, 3877618.8799599423),
       Point(13388824.565050386, 3877620.476645117), Point(13388823.214529714, 3877627.8729228196)},
      {Point(13388850.31988399, 3877640.977072904), Point(13388849.891306637, 3877632.2409587177),
       Point(13388848.355161773, 3877630.679094635), Point(13388779.541046051, 3877626.1568713714),
       Point(13388778.3100396, 3877626.9525231253), Point(13388775.374635553, 3877634.8088330356),
       Point(13388777.39787929, 3877637.9660580857), Point(13388848.078277094, 3877643.164642153)},
      {Point(13388768.359138895, 3877628.144749175), Point(13388768.777014626, 3877621.7247608397),
       Point(13388767.20036562, 3877619.838774188), Point(13388759.609340262, 3877618.975776001),
       Point(13388757.475679567, 3877620.72077982), Point(13388756.90121772, 3877627.34229127),
       Point(13388758.70654354, 3877629.4310200238), Point(13388766.391159521, 3877629.883091093)},
      {Point(13388762.147375304, 3877633.8970327205), Point(13388759.507911108, 3877621.709332798),
       Point(13388758.329729524, 3877620.701145656), Point(13388654.414831385, 3877614.658702894),
       Point(13388654.336637424, 3877614.69322552), Point(13388649.564210305, 3877620.5805437258),
       Point(13388651.387312314, 3877625.0086345845), Point(13388759.54446656, 3877636.788064386)},
      {Point(13388654.952941336, 3877618.7343446855), Point(13388646.385801775, 3877623.4201370324),
       Point(13388642.460739406, 3877621.0613246406), Point(13388642.692938047, 3877601.638806315),
       Point(13388644.826625269, 3877599.6809815094), Point(13388652.470357422, 3877600.247322524),
       Point(13388653.879013304, 3877601.6272270787), Point(13388655.418562343, 3877617.8568790997)},
      {Point(13388880.370498562, 3877614.530210331), Point(13388882.055921659, 3877620.62596337),
       Point(13388884.040099949, 3877621.8564826027), Point(13388894.993459038, 3877619.740694212),
       Point(13388895.973180663, 3877618.6733302455), Point(13388896.635966176, 3877612.3987006946),
       Point(13388893.897630129, 3877609.8492654627), Point(13388881.979083586, 3877611.976939431)},
      {Point(13388945.898281306, 3877608.101396902), Point(13388946.258900464, 3877601.261216692),
       Point(13388947.82205416, 3877599.6926954803), Point(13388954.711308219, 3877599.3058035374),
       Point(13388956.556069935, 3877600.857221336), Point(13388957.3247082, 3877607.4409611486),
       Point(13388955.668690884, 3877609.5275793113), Point(13388948.300167471, 3877610.362863082)},
      {Point(13389006.934254706, 3877592.8714147382), Point(13389007.3367357, 3877599.021827494),
       Point(13389010.473381607, 3877601.07001248), Point(13389016.610706707, 3877598.8400904275),
       Point(13389017.303197604, 3877597.859641748), Point(13389017.350217689, 3877591.668203464),
       Point(13389014.109912237, 3877589.3556478913), Point(13389007.783931786, 3877591.5840472323)}};
  Point start_pt(13220709.655, 3760420.094);


  Point offset_pt = outline[0];
  for (auto &pt : outline) pt = Point(pt.x() - offset_pt.x(), pt.y() - offset_pt.y());
  for (auto &hole : holes)
    for (auto &pt : hole) pt = Point(pt.x() - offset_pt.x(), pt.y() - offset_pt.y());
  start_pt = Point(start_pt.x() - offset_pt.x(), start_pt.y() - offset_pt.y());

  work.SetPolygon(outline, holes);

  double rotation = 86.2868152690651;
  double interval = 2.3983484396072385;
  double offset   = -0.1682206309629235;
  work.RotateIt(rotation);
  Point center = work.GetCenter();
  start_pt    = TF_2::Rotate(start_pt, center, rotation);

  std::vector<Segment> tracks;
  work.GetTracks(start_pt, interval, offset, tracks);
  std::vector<Trapezoid_2> zones;
  work.GetMonotoneZones(start_pt, zones);
  std::cout << zones.size() << std::endl;
  std::vector<Polygon> zones_pg;
  for (const auto &z : zones) zones_pg.push_back(z.polygon_);

  // std::vector<Point> path;
  // double             dist = work.ShortestPath(Point(3.1, 11.86), Point(5.5, 2), path);
  // std::cout << dist << " " << path.size() << std::endl;
  // std::vector<Segment> path_segs;
  // for (size_t i = 0; i < path.size() - 1; ++i)
  // {
  //   std::cout << String_2::PointToString(path[i]) << std::endl;
  //   path_segs.push_back(Segment(path[i], path[i + 1]));
  // }

  // work.RotateIt(45.0);

  if (1)
  {
    Polygon work_pg = work.GetPolygon();
    vis_ptr->Clear();
    vis_ptr->NewFile("work-pg", {work_pg});
    vis_ptr->NewFile("tracks", tracks);
    vis_ptr->NewFile("zones", zones_pg);
    // vis_ptr->NewFile("path", path_segs);
    vis_ptr->Show();

    return 1;
  }

  if (0)
  {
    std::cout << " ==================  Boost Graph Dijkstra ================== " << std::endl;
    Graph g;

    Point  p0(0, 0);
    Point  p1(1, 0);
    Point  p2(3, 1);
    Point  p3(-3, 1);
    Point  p4(1, -4);
    Vertex v0 = boost::add_vertex(p0, g);
    Vertex v1 = boost::add_vertex(p1, g);
    Vertex v2 = boost::add_vertex(p2, g);
    Vertex v3 = boost::add_vertex(p3, g);
    Vertex v4 = boost::add_vertex(p4, g);

    boost::add_edge(v0, v1, bg::distance(p0, p1), g);
    boost::add_edge(v0, v2, bg::distance(p0, p2), g);
    boost::add_edge(v0, v3, bg::distance(p0, p3), g);
    // boost::add_edge(v0, v4, bg::distance(p0, p4), g);
    boost::add_edge(v1, v2, bg::distance(p1, p2), g);
    boost::add_edge(v1, v3, bg::distance(p1, p3), g);
    boost::add_edge(v1, v4, bg::distance(p1, p4), g);
    boost::add_edge(v2, v3, bg::distance(p2, p3), g);
    boost::add_edge(v2, v4, bg::distance(p2, p4), g);
    boost::add_edge(v3, v4, bg::distance(p3, p4), g);
    std::cout << boost::num_vertices(g) << ", " << boost::num_edges(g) << std::endl;

    std::vector<Vertex> v_predecessor(boost::num_vertices(g));
    // store distance from src to all others
    std::vector<double> v_distance(boost::num_vertices(g));

    Vertex start_vertex = v0;
    boost::dijkstra_shortest_paths(g, start_vertex,
                                   boost::predecessor_map(&v_predecessor[0]).distance_map(&v_distance[0]));

    std::cout << "Shortest path distances from vertex v0:" << std::endl;
    std::cout << "Distance to v" << v4 << ": " << v_distance[v4] << std::endl;
    // boost::graph_traits<Graph>::vertex_iterator vi, vend;
    // for (boost::tie(vi, vend) = boost::vertices(g); vi != vend; ++vi)
    // {
    //   Vertex v = *vi;
    //   std::cout << "Distance to vertex " << v << ": " << v_distance[v] << std::endl;
    // }

    // Print the shortest path from vertex v1 to vertex v4
    std::cout << "Shortest path from vertex v1 to vertex v4:" << std::endl;
    std::cout << "Path: ";
    Vertices           vertices = boost::get(boost::vertex_name, g);
    Vertex             v        = v4;
    std::vector<Point> path;
    while (v != start_vertex)
    {
      path.push_back(vertices[v]);
      v = v_predecessor[v];
    }
    path.push_back(vertices[v]);
    std::reverse(path.begin(), path.end());
    for (const auto &pt : path) std::cout << String_2::PointToString(pt) << " -> ";
    std::cout << std::endl;

    // boost::clear_vertex(v1, g);
    // boost::remove_vertex(v1, g);
    // std::cout << boost::num_vertices(g) << ", " << boost::num_edges(g) << std::endl;
  }

  return 0;
}