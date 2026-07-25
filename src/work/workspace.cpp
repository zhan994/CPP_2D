#include "work/workspace.h"

Trapezoid::Trapezoid(const Polygon &p, const Segment &left, const Segment &right)
    : polygon_(p), left_(left), right_(right)
{
  left_state_  = !bg::equals(left.first, left.second);
  right_state_ = !bg::equals(right.first, right.second);
  left_x_      = left_.first.x();
  right_x_     = right_.first.x();
}

Work::Work()
{
  std::cout << " Create Work Object." << std::endl;
}

void Work::SetPolygon(const std::vector<Point> &outline, const std::vector<std::vector<Point>> &holes)
{
  work_pg_ = Utility::SetData(outline);
  for (const auto &h : holes) AddHole(h);

  bg::centroid(work_pg_, centroid_);

  // note: 与旋转后的统计重复，删除这部分逻辑
  // all_pts_.clear();
  // all_pts_id_.clear();
  // all_pts_.insert(all_pts_.end(), work_pg_.outer().begin(), work_pg_.outer().end());
  // for (const auto &inner : work_pg_.inners()) all_pts_.insert(all_pts_.end(), inner.begin(), inner.end());
  // for (const auto &pt : all_pts_) all_pts_id_.push_back(String::PointToString(pt));

  // outline_segs_.clear();
  // holes_segs_.clear();
  // all_segs_.clear();
  // for (size_t i = 0; i < work_pg_.outer().size() - 1; ++i)
  //   outline_segs_.push_back(Segment(work_pg_.outer().at(i), work_pg_.outer().at(i + 1)));

  // all_segs_.insert(all_segs_.end(), outline_segs_.begin(), outline_segs_.end());

  // for (const auto &hole : work_pg_.inners())
  // {
  //   std::vector<Segment> hole_segs;
  //   for (size_t i = 0; i < hole.size() - 1; ++i) hole_segs.push_back(Segment(hole.at(i), hole.at(i + 1)));

  //   holes_segs_.push_back(hole_segs);
  //   all_segs_.insert(all_segs_.end(), hole_segs.begin(), hole_segs.end());
  // }

  // registered_pts_map_.clear();
  // registered_pts_pairs_.clear();
  // RegisterMap();

  // InitGraph();
}

void Work::AddHole(const std::vector<Point> &hole)
{
  Polygon hole_pg = Utility::SetData(hole);
  if (bg::intersects(work_pg_, hole_pg))
  {
    std::vector<Polygon> new_work_pgs;
    Bool::Difference(work_pg_, hole_pg, new_work_pgs);
    work_pg_ = new_work_pgs.front();
  }
}

void Work::RotateIt(double angle)
{
  work_pg_ = TF::Rotate(work_pg_, centroid_, angle);

  all_pts_.clear();
  all_pts_id_.clear();
  all_pts_.insert(all_pts_.end(), work_pg_.outer().begin(), work_pg_.outer().end());
  for (const auto &inner : work_pg_.inners()) all_pts_.insert(all_pts_.end(), inner.begin(), inner.end());
  for (const auto &pt : all_pts_) all_pts_id_.push_back(String::PointToString(pt));

  outline_segs_.clear();
  holes_segs_.clear();
  all_segs_.clear();
  for (size_t i = 0; i < work_pg_.outer().size() - 1; ++i)
    outline_segs_.push_back(Segment(work_pg_.outer().at(i), work_pg_.outer().at(i + 1)));

  all_segs_.insert(all_segs_.end(), outline_segs_.begin(), outline_segs_.end());

  for (const auto &hole : work_pg_.inners())
  {
    std::vector<Segment> hole_segs;
    for (size_t i = 0; i < hole.size() - 1; ++i) hole_segs.push_back(Segment(hole.at(i), hole.at(i + 1)));

    holes_segs_.push_back(hole_segs);
    all_segs_.insert(all_segs_.end(), hole_segs.begin(), hole_segs.end());
  }

  registered_pts_map_.clear();
  registered_pts_pairs_.clear();
  RegisterMap();

  InitGraph();
}

void Work::RegisterMap()
{
  size_t num_edges = work_pg_.outer().size() - 1;
  for (size_t i = 0; i < num_edges; ++i)
  {
    Point  pt    = work_pg_.outer().at(i);
    size_t edge0 = i == 0 ? num_edges - 1 : i - 1;
    size_t edge1 = i;

    std::string pt_str    = String::PointToString(pt);
    std::string edge0_str = "<0," + std::to_string(edge0) + ">";
    std::string edge1_str = "<0," + std::to_string(edge1) + ">";
    if (registered_pts_map_.find(pt_str) == registered_pts_map_.end())
    {
      registered_pts_map_[pt_str] = {edge0_str, edge1_str};
    } else
    {
      registered_pts_map_[pt_str].push_back(edge0_str);
      registered_pts_map_[pt_str].push_back(edge1_str);
    }
  }

  size_t num_holes = work_pg_.inners().size();
  for (size_t i = 0; i < num_holes; ++i)
  {
    size_t num_edges = work_pg_.inners().at(i).size() - 1;
    for (size_t j = 0; j < num_edges; ++j)
    {
      Point  pt    = work_pg_.inners().at(i).at(j);
      size_t edge0 = j == 0 ? num_edges - 1 : j - 1;
      size_t edge1 = j;

      std::string pt_str    = String::PointToString(pt);
      std::string edge0_str = "<" + std::to_string(i + 1) + "," + std::to_string(edge0) + ">";
      std::string edge1_str = "<" + std::to_string(i + 1) + "," + std::to_string(edge1) + ">";
      if (registered_pts_map_.find(pt_str) == registered_pts_map_.end())
      {
        registered_pts_map_[pt_str] = {edge0_str, edge1_str};
      } else
      {
        registered_pts_map_[pt_str].push_back(edge0_str);
        registered_pts_map_[pt_str].push_back(edge1_str);
      }
    }
  }

  if (0)
  {
    std::cout << " [Debug] =====================> Work::RegisterMap" << std::endl;
    for (const auto &kv : registered_pts_map_)
    {
      for (const auto &e : kv.second)
      {
        std::cout << kv.first << "," << e << std::endl;
      }
    }
  }
}

void Work::Register(const std::string &pt_str, const std::string &edge_str)
{
  if (registered_pts_map_.find(pt_str) == registered_pts_map_.end())
  {
    registered_pts_map_[pt_str] = {edge_str};
  } else
  {
    registered_pts_map_[pt_str].push_back(edge_str);
  }
}

void Work::InitGraph()
{
  graph_ = Graph();
  graph_map_.clear();
  for (size_t i = 0; i < all_pts_.size(); ++i)
  {
    Vertex v                   = boost::add_vertex(all_pts_[i], graph_);
    graph_map_[all_pts_id_[i]] = v;
  }

  for (size_t i = 0; i < all_pts_.size(); ++i)
  {
    Point pt1 = all_pts_[i];
    for (size_t j = i + 1; j < all_pts_.size(); ++j)
    {
      Point pt2 = all_pts_[j];
      if (ValidPtsPair(pt1, pt2))
      {
        double dist = bg::distance(pt1, pt2);
        boost::add_edge(i, j, dist, graph_);
      }
    }
  }

  graph_bak_ = graph_;
}

bool Work::ValidPtsPair(const Point &pt1, const Point &pt2)
{
  std::string pair_id = String::PointToString(pt1) + "_" + String::PointToString(pt2);
  bool        val;
  if (registered_pts_pairs_.find(pair_id) != registered_pts_pairs_.end())
  {
    val = registered_pts_pairs_[pair_id];
  } else
  {
    if (PointsOnSameEdge(pt1, pt2))
    {
      val = true;
    } else
    {
      Segment seg(pt1, pt2);
      val = Bool::Within(Buffer::Shrink(seg), work_pg_);
    }
    registered_pts_pairs_[pair_id] = val;
  }

  return val;
}

bool Work::PointsOnSameEdge(const Point &pt1, const Point &pt2)
{
  std::string pt1_str = String::PointToString(pt1);
  std::string pt2_str = String::PointToString(pt2);
  if (registered_pts_map_.find(pt1_str) != registered_pts_map_.end() &&
      registered_pts_map_.find(pt2_str) != registered_pts_map_.end())
  {
    std::vector<std::string> res1 = registered_pts_map_[pt1_str];
    std::vector<std::string> res2 = registered_pts_map_[pt2_str];

    std::set<std::string> e1(res1.begin(), res1.end());
    std::set<std::string> e2(res2.begin(), res2.end());
    if (e1.size() <= e2.size())
    {
      for (const auto &s : e1)
      {
        if (e2.find(s) != e2.end())
          return true;
      }
      return false;
    } else
    {
      for (const auto &s : e2)
      {
        if (e1.find(s) != e1.end())
          return true;
      }
      return false;
    }
  }

  return false;
}

double Work::ShortestPath(const Point &start, const Point &end, std::vector<Point> &path)
{
  if (ValidPtsPair(start, end))
  {
    path = {start, end};
    return bg::distance(start, end);
  }

  std::string start_id  = String::PointToString(start);
  bool        add_start = false;
  if (std::find(all_pts_id_.begin(), all_pts_id_.end(), start_id) == all_pts_id_.end())
  {
    add_start            = true;
    Vertex v             = boost::add_vertex(start, graph_);
    graph_map_[start_id] = v;
    for (size_t i = 0; i < all_pts_.size(); ++i)
    {
      Point pt = all_pts_[i];
      if (ValidPtsPair(start, pt))
      {
        double dist = bg::distance(start, pt);
        boost::add_edge(v, i, dist, graph_);
      }
    }
  }

  std::string end_id  = String::PointToString(end);
  bool        add_end = false;
  if (std::find(all_pts_id_.begin(), all_pts_id_.end(), end_id) == all_pts_id_.end())
  {
    add_end            = true;
    Vertex v           = boost::add_vertex(end, graph_);
    graph_map_[end_id] = v;
    for (size_t i = 0; i < all_pts_.size(); ++i)
    {
      Point pt = all_pts_[i];
      if (ValidPtsPair(end, pt))
      {
        double dist = bg::distance(end, pt);
        boost::add_edge(v, i, dist, graph_);
      }
    }
  }

  // auto   t1   = std::chrono::steady_clock::now();
  double dist = Dijkstra(start_id, end_id, path);
  // note: <fix> boost remove_vertex bug
  graph_ = graph_bak_;

  // auto                          t2       = std::chrono::steady_clock::now();
  // std::chrono::duration<double> elapsed1 = t2 - t1;
  // std::cout << boost::num_vertices(graph_) << " ***************** dijkstra: " << 1000 * elapsed1.count()
  //           << " ms ***************** " << std::endl;
  return dist;
}

double Work::Dijkstra(const std::string &start_id, const std::string &end_id, std::vector<Point> &path)
{
  // store predecessor vertex
  std::vector<Vertex> v_predecessor(boost::num_vertices(graph_));
  // store distance from src to all others
  std::vector<double> v_distance(boost::num_vertices(graph_));

  Vertices vertices     = boost::get(boost::vertex_name, graph_);
  Vertex   start_vertex = graph_map_[start_id];
  Vertex   end_vertex   = graph_map_[end_id];
  boost::dijkstra_shortest_paths(
      graph_, start_vertex,
      boost::predecessor_map(
          boost::make_iterator_property_map(v_predecessor.begin(), boost::get(boost::vertex_index, graph_)))
          .distance_map(
              boost::make_iterator_property_map(v_distance.begin(), boost::get(boost::vertex_index, graph_))));

  Vertex v = end_vertex;
  while (v != start_vertex)
  {
    path.push_back(vertices[v]);
    v = v_predecessor[v];
  }
  path.push_back(vertices[v]);
  std::reverse(path.begin(), path.end());

  return v_distance[end_vertex];
}

double Work::GetTracks(const Point &start_pt, double interval, double offset, std::vector<Segment> &out)
{
  std::vector<double> bbox;
  Buffer::BBox(work_pg_, bbox);
  double x_min = bbox[0];
  double y_min = bbox[1];
  double x_max = bbox[2];
  double y_max = bbox[3];

  bool   from_left    = start_pt.x() < (x_min + x_max) / 2.0;
  double offset_right = fmod(x_max - x_min - offset, interval);
  offset              = from_left ? offset : offset_right;

  std::vector<Segment> tracks;
  double               cur_x = x_min + offset;
  while (cur_x < x_max && cur_x > x_min)
  {
    Point p1(cur_x, y_min);
    Point p2(cur_x, y_max);

    Segment             cur_seg(p1, p2);
    std::vector<double> intersections_y;
    for (size_t i = 0; i < outline_segs_.size(); ++i)
    {
      Segment seg       = outline_segs_[i];
      Point   s         = seg.first;
      Point   e         = seg.second;
      double  seg_x_min = std::min(s.x(), e.x());
      double  seg_x_max = std::max(s.x(), e.x());
      if (cur_x >= seg_x_min && cur_x <= seg_x_max)
      {
        if (bg::intersects(cur_seg, seg))
        {
          std::vector<Point> pts;
          Bool::Intersection(cur_seg, seg, pts);

          Point pt = pts.front();
          intersections_y.push_back(pt.y());
          std::string seg_str = "<0," + std::to_string(i) + ">";
          Register(String::PointToString(pt), seg_str);
        }
      }
    }

    for (size_t i = 0; i < holes_segs_.size(); ++i)
    {
      for (size_t j = 0; j < holes_segs_[i].size(); ++j)
      {
        Segment seg       = holes_segs_[i][j];
        Point   s         = seg.first;
        Point   e         = seg.second;
        double  seg_x_min = std::min(s.x(), e.x());
        double  seg_x_max = std::max(s.x(), e.x());
        if (cur_x >= seg_x_min && cur_x <= seg_x_max)
        {
          if (bg::intersects(cur_seg, seg))
          {
            std::vector<Point> pts;
            Bool::Intersection(cur_seg, seg, pts);

            Point pt = pts.front();
            intersections_y.push_back(pt.y());
            std::string seg_str = "<" + std::to_string(i + 1) + "," + std::to_string(j) + ">";
            Register(String::PointToString(pt), seg_str);
          }
        }
      }
    }

    std::sort(intersections_y.begin(), intersections_y.end());
    for (size_t i = 0; i < intersections_y.size() - 1; ++i)
    {
      Segment seg(Point(cur_x, intersections_y[i]), Point(cur_x, intersections_y[i + 1]));
      Segment shrink_seg = Buffer::Shrink(seg);
      if (Bool::Within(shrink_seg, work_pg_))
        tracks.push_back(seg);
    }

    cur_x += interval;
  }
  // note: <fix> empty tracks
  if (tracks.empty())
  {
    std::cout << " Error: No valid tracks!!!" << std::endl;
    return 0;
  }

  for (const auto &track : tracks)
  {
    if (out.empty() || !bg::equals(out.back().second, track.first))
    {
      out.push_back(track);
    } else
    {
      out.back() = Utility::MergeVerticalSegments(out.back(), track);
    }
  }

  // note: <fix> start_x error due to tracks
  double start_x = from_left ? out.front().first.x() : out.back().first.x();

  return start_x;
}

void Work::GetMonotoneZones(const Point &start_pt, std::vector<Trapezoid> &out)
{
  std::vector<Segment> sweep_lines;
  VerticalSweepLines(start_pt, sweep_lines);

  std::vector<std::vector<Trapezoid>> trapezoids;
  for (size_t i = 0; i < sweep_lines.size() - 1; ++i)
  {
    Segment left_line  = sweep_lines[i];
    Segment right_line = sweep_lines[i + 1];

    std::vector<Segment> legs_data;
    for (const auto &seg : all_segs_)
    {
      // note: intersection1 and intersection2 can be a same point.
      bool left_inter_state  = bg::intersects(left_line, seg);
      bool right_inter_state = bg::intersects(right_line, seg);
      if (left_inter_state && right_inter_state)
      {
        std::vector<Point> left_inter_pts, right_inter_pts;
        Bool::Intersection(left_line, seg, left_inter_pts);
        Bool::Intersection(right_line, seg, right_inter_pts);
        legs_data.push_back(Segment(left_inter_pts.front(), right_inter_pts.front()));
      }
    }

    // note: <fix> legs_data empty
    if (legs_data.empty())
      continue;
    std::sort(legs_data.begin(), legs_data.end(), Utility::CompSegment);
    // std::cout << "debug leg" << std::endl;
    // for (const auto leg : legs_data) std::cout << String::SegmentToString(leg) << std::endl;

    std::vector<Trapezoid> trapezoid_col;
    for (size_t j = 0; j < legs_data.size() - 1; ++j)
    {
      Segment down_leg = legs_data[j];
      Segment up_leg   = legs_data[j + 1];

      Polygon pg;
      pg.outer().push_back(down_leg.first);
      pg.outer().push_back(down_leg.second);
      pg.outer().push_back(up_leg.second);
      pg.outer().push_back(up_leg.first);
      pg.outer().push_back(down_leg.first);
      bg::correct(pg);

      Polygon shrink_pg;
      if (Buffer::Shrink(pg, shrink_pg))
      {
        if (bg::within(shrink_pg, work_pg_))
        {
          Trapezoid trapezoid(pg, Segment(down_leg.first, up_leg.first), Segment(down_leg.second, up_leg.second));
          trapezoid_col.push_back(trapezoid);
        }
      }
    }

    trapezoids.push_back(trapezoid_col);
  }

  for (size_t i = 0; i < trapezoids.size() - 1; ++i)
  {
    std::vector<Trapezoid> &left_col  = trapezoids[i];
    std::vector<Trapezoid> &right_col = trapezoids[i + 1];
    for (const auto &left : left_col)
    {
      bool merge = false;
      for (auto &right : right_col)
      {
        // std::cout << "*********************** judge" << String::SegmentToString(left.right_) << " "
        //           << String::SegmentToString(right.left_) << " " << left.right_state_
        //           << bg::equals(left.right_, right.left_) << std::endl;
        if (left.right_state_ && bg::equals(left.right_, right.left_))
        {
          if (start_pt.x() != left.right_x_)
          {
            right.left_       = left.left_;
            right.left_x_     = left.left_x_;
            right.left_state_ = left.left_state_;

            std::vector<Polygon> union_pgs;
            Bool::Union(left.polygon_, right.polygon_, union_pgs);
            right.polygon_ = union_pgs.front();
            merge          = true;
            break;
          }
        }
      }
      if (!merge)
        out.push_back(left);
    }
  }
  for (const auto &right : trapezoids.back()) out.push_back(right);
}

void Work::VerticalSweepLines(const Point &start_pt, std::vector<Segment> &out)
{
  std::vector<double> v_x;
  v_x.push_back(start_pt.x());
  for (const auto &pt : work_pg_.outer()) v_x.push_back(pt.x());

  for (const auto &hole : work_pg_.inners())
    for (const auto &pt : hole) v_x.push_back(pt.x());

  std::set<double> s_x(v_x.begin(), v_x.end());
  v_x.assign(s_x.begin(), s_x.end());
  std::sort(v_x.begin(), v_x.end());

  std::vector<double> bbox;
  Buffer::BBox(work_pg_, bbox);
  double y_min = bbox[1];
  double y_max = bbox[3];
  for (const auto &x : v_x) out.push_back(Segment(Point(x, y_min - 1), Point(x, y_max + 1)));
}
