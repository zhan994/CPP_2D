#include "work/route_2.h"

Route_2::Route_2()
{}

void Route_2::SetWork(const std::vector<Point> &outline, const std::vector<std::vector<Point>> &holes)
{
  outline_   = outline;
  holes_     = holes;
  origin_pt_ = outline[0];

  for (auto &pt : outline_)
    pt = Point(Utility_2::DRound(pt.x() - origin_pt_.x(), 6), Utility_2::DRound(pt.y() - origin_pt_.y(), 6));

  for (auto &hole : holes_)
    for (auto &pt : hole)
      pt = Point(Utility_2::DRound(pt.x() - origin_pt_.x(), 6), Utility_2::DRound(pt.y() - origin_pt_.y(), 6));

  work_.SetPolygon(outline_, holes_);
}

void Route_2::SetParam(const Point &start_pt, double rotation, double interval, double offset)
{
  start_pt_ = Point(start_pt.x() - origin_pt_.x(), start_pt.y() - origin_pt_.y());
  rotation_ = rotation; // deg
  interval_ = interval;
  offset_   = offset;
}

void Route_2::CollectPivotalTrackCandidate(const std::vector<Segment> &sorted_tracks, std::vector<Segment> &out)
{
  std::vector<std::vector<Segment>> track_hist;
  double                            prev_x;
  bool                              prev_x_state = false;
  for (const auto &track : sorted_tracks)
  {
    double cur_x = track.first.x();
    if (!prev_x_state || prev_x != cur_x)
    {
      prev_x_state = true;
      prev_x       = cur_x;
      track_hist.push_back({track});
    } else
    {
      track_hist.back().push_back(track);
    }
  }

  for (size_t i = 1; i < track_hist.size() - 1; ++i)
  {
    int num_0 = track_hist[i - 1].size();
    int num_1 = track_hist[i].size();
    int num_2 = track_hist[i + 1].size();
    if (num_1 < num_0 || num_1 < num_2)
      out.insert(out.end(), track_hist[i].begin(), track_hist[i].end());
  }

  // std::cout << "candi" << std::endl;
  // for (const auto &can : out) std::cout << String_2::SegmentToString(can) << std::endl;
}

void Route_2::DivideTracksIntoCells(std::vector<Segment> &          tracks,
                                    const std::vector<Trapezoid_2> &zones,
                                    std::vector<Cell_2> &           out)
{
  std::vector<Segment> pivotal_tracks;
  std::vector<Segment> pivotal_track_candidates;
  CollectPivotalTrackCandidate(tracks, pivotal_track_candidates);
  // std::cout << "pivo_candi_size: " << pivotal_track_candidates.size() << std::endl;

  for (const auto &zone : zones)
  {
    Cell_2 cell;
    for (const auto &track : tracks)
    {
      double x = track.first.x();
      if (x >= zone.left_x_ && x <= zone.right_x_)
      {
        // Debug(track);
        bool end1_on_zone = bg::distance(track.first, zone.polygon_) < 1e-8;
        bool end2_on_zone = bg::distance(track.second, zone.polygon_) < 1e-8;
        if (end1_on_zone && end2_on_zone)
          cell.AddTrack(track);
      }
    }
    cell.SortTracks();

    // note: check if front/back track is pivotal
    if (!cell.sorted_tracks_.empty() &&
        std::find_if(pivotal_track_candidates.begin(), pivotal_track_candidates.end(), [&](const Segment &seg) {
          return bg::equals(cell.sorted_tracks_.front(), seg);
        }) != pivotal_track_candidates.end())
    {
      pivotal_tracks.push_back(cell.sorted_tracks_.front());
      cell.sorted_tracks_.assign(cell.sorted_tracks_.begin() + 1, cell.sorted_tracks_.end());
    }

    if (!cell.sorted_tracks_.empty() &&
        std::find_if(pivotal_track_candidates.begin(), pivotal_track_candidates.end(), [&](const Segment &seg) {
          return bg::equals(cell.sorted_tracks_.back(), seg);
        }) != pivotal_track_candidates.end())
    {
      pivotal_tracks.push_back(cell.sorted_tracks_.back());
      cell.sorted_tracks_.assign(cell.sorted_tracks_.begin(), cell.sorted_tracks_.end() - 1);
    }

    // std::cout << "add pivotal" << std::endl;
    // for (const auto &track : cell.sorted_tracks_)
    // {
    //   std::cout << String_2::SegmentToString(track) << std::endl;
    // }

    for (const auto &track : cell.sorted_tracks_)
      tracks.erase(
          std::remove_if(tracks.begin(), tracks.end(), [&](const Segment &seg) { return bg::equals(track, seg); }),
          tracks.end());

    if (!cell.sorted_tracks_.empty())
    {
      cell.GetPaths(work_);
      out.push_back(cell);
    }
  }

  for (const auto &track : pivotal_tracks)
  {
    // std::cout << "pivo: " << String_2::SegmentToString(track) << std::endl;
    Cell_2 cell;
    cell.AddTrack(track);
    cell.GetPaths(work_);
    out.push_back(cell);
    tracks.erase(
        std::remove_if(tracks.begin(), tracks.end(), [&](const Segment &seg) { return bg::equals(track, seg); }),
        tracks.end());
  }

  if (!tracks.empty())
  {
    std::cout << "Error: tracks remained." << std::endl;
    for (const auto &track : tracks)
    {
      std::cout << String_2::SegmentToString(track) << std::endl;
    }
    out.clear();
  }
}

void Route_2::GetStartCellIndex(
    const std::vector<Cell_2> &cells, const Point &start_point, double start_x, size_t &sci, size_t &sni)
{
  double closest_dist = std::numeric_limits<double>::max();
  for (size_t i = 0; i < cells.size(); ++i)
  {
    Cell_2 cell = cells[i];
    for (size_t j = 0; j < cell.NumNodes(); ++j)
    {
      Point cell_start = cell.paths_[j][0];
      if (std::abs(cell_start.x() - start_x) < 1e-6)
      {
        double dist = bg::distance(start_point, cell_start);
        if (dist < closest_dist)
        {
          closest_dist = dist;
          sci          = i;
          sni          = j;
        }
      }
    }
  }
}

void Route_2::PlanWithRotation(std::vector<Point> &path, std::vector<WaypointType> &types, std::vector<int> &line_nums)
{
  work_.RotateIt(rotation_);
  Point center = work_.GetCenter();
  start_pt_    = TF_2::Rotate(start_pt_, center, rotation_);

  auto t1 = std::chrono::steady_clock::now();

  std::vector<Segment> tracks;
  double               start_x = work_.GetTracks(start_pt_, interval_, offset_, tracks);
  std::cout << " tracks size: " << tracks.size() << std::endl;
  // note: add empty tracks judge
  if (tracks.empty())
    return;
  auto t2 = std::chrono::steady_clock::now();

  std::vector<Trapezoid_2> zones;
  work_.GetMonotoneZones(start_pt_, zones);
  std::cout << " zones size: " << zones.size() << std::endl;

  auto t3 = std::chrono::steady_clock::now();

  std::vector<Cell_2> cells;
  DivideTracksIntoCells(tracks, zones, cells);
  std::cout << " cells size: " << cells.size() << std::endl;

  auto t4 = std::chrono::steady_clock::now();

  dfs_ = DFS_2(work_, cells, 10000, 25);
  size_t sci, sni;
  GetStartCellIndex(cells, start_pt_, start_x, sci, sni);
  // Stop();
  dfs_.FindMinimunPath(sci, sni, interval_, path, types);
  GetLineNums(path, types, line_nums);
  for (auto &pt : path)
  {
    pt = TF_2::Rotate(pt, center, -rotation_);
    pt = Point(pt.x() + origin_pt_.x(), pt.y() + origin_pt_.y());
  }
  auto t5 = std::chrono::steady_clock::now();

  std::chrono::duration<double> elapsed1 = t2 - t1;
  std::chrono::duration<double> elapsed2 = t3 - t2;
  std::chrono::duration<double> elapsed3 = t4 - t3;
  std::chrono::duration<double> elapsed4 = t5 - t4;
  std::chrono::duration<double> elapsed5 = t5 - t1;
  std::cout << " ***************** Get tracks: " << 1000 * elapsed1.count() << " ms ***************** " << std::endl;
  std::cout << " ***************** Get zones: " << 1000 * elapsed2.count() << " ms ***************** " << std::endl;
  std::cout << " ***************** Get cells: " << 1000 * elapsed3.count() << " ms ***************** " << std::endl;
  std::cout << " ***************** DFS search: " << 1000 * elapsed4.count() << " ms ***************** " << std::endl;
  std::cout << " ***************** Total cost: " << 1000 * elapsed5.count() << " ms ***************** " << std::endl;
}

void Route_2::GetLineNums(const std::vector<Point> &       path,
                          const std::vector<WaypointType> &types,
                          std::vector<int> &               line_nums)
{
  line_nums.clear();

  int    cur_line_num = -1;
  double cur_x        = std::numeric_limits<double>::max();
  for (size_t i = 0; i < path.size(); ++i)
  {
    if (types[i] == WaypointType::NORMAL_START)
    {
      if (cur_line_num < 0)
      {
        cur_line_num = 0;
        cur_x        = path[i].x();
        line_nums.push_back(cur_line_num);
      } else if (std::abs(path[i].x() - cur_x) < 1e-6)
      {
        line_nums.push_back(cur_line_num);
      } else
      {
        cur_x = path[i].x();
        cur_line_num += 1;
        line_nums.push_back(cur_line_num);
      }
    } else if (i > 0 && types[i - 1] == WaypointType::NORMAL_START)
    {
      line_nums.push_back(cur_line_num);
    } else
    {
      line_nums.push_back(-1);
    }
  }
}