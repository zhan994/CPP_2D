#include "work/cell.h"

Cell::Cell() : single_pt_state_(false)
{}

void Cell::AddTrack(const Segment &track)
{
  if (single_pt_state_)
    return;

  tracks_.push_back(track);
}

void Cell::SortTracks()
{
  sorted_tracks_ = tracks_;
  std::sort(sorted_tracks_.begin(), sorted_tracks_.end(), Utility::CompSegment);
  // std::cout << sorted_tracks_.size() << std::endl;
  // for (const auto &t : sorted_tracks_) std::cout << String::SegmentToString(t) << std::endl;
}

int Cell::NumNodes()
{
  if (single_pt_state_)
  {
    return 1;
  } else if (sorted_tracks_.size() == 1)
  {
    return 2;
  } else
  {
    return 4;
  }
}

void Cell::SetSinglePoint(const Point &pt)
{
  single_pt_       = pt;
  single_pt_state_ = true;

  tracks_.clear();
  sorted_tracks_.clear();
  paths_          = {{pt}};
  paths_length_   = {std::pair<double, double>(0., 0.)};
  waypoint_types_ = {{WaypointType::TRANSITION}};
}

void Cell::GetPathEnd(size_t ni, std::vector<Point> &out)
{
  out = {paths_[ni].front(), paths_[ni].back()};
}

std::vector<Point> Cell::GetPaths(Work &work, bool from_left, bool from_up, bool simplified)
{
  std::vector<Segment> merged_tracks;
  for (size_t i = 0; i < sorted_tracks_.size(); ++i)
  {
    Segment track = sorted_tracks_[i];
    if (merged_tracks.empty() || merged_tracks.back().first.x() != track.first.x())
    {
      merged_tracks.push_back(track);
    } else
    {
      merged_tracks.back() = Utility::MergeVerticalSegments(merged_tracks.back(), track);
    }
  }

  std::vector<Point>        ret_path;
  std::vector<WaypointType> wp_types;
  int                       num_tracks   = merged_tracks.size();
  double                    total_length = 0.0, total_connect_length = 0.0;

  Point previous_end;
  bool  previous_end_state = false;
  for (size_t i = 0; i < num_tracks; ++i)
  {
    Segment current_track = from_left ? merged_tracks[i] : merged_tracks[num_tracks - i - 1];
    Point   current_start = from_up ? current_track.second : current_track.first;
    Point   current_end   = from_up ? current_track.first : current_track.second;
    total_length += bg::distance(current_start, current_end);

    std::vector<Point> connect_path;
    double             connect_length = 0.0;
    if (previous_end_state)
    {
      if (simplified)
      {
        connect_path.push_back(previous_end);
        connect_path.push_back(current_start);
        connect_length = bg::distance(previous_end, current_start);
      } else
      {
        connect_length = work.ShortestPath(previous_end, current_start, connect_path);
      }

      total_connect_length += connect_length;
      total_length += connect_length;
      connect_path.assign(connect_path.begin() + 1, connect_path.end() - 1);
      if (!connect_path.empty())
      {
        ret_path.insert(ret_path.end(), connect_path.begin(), connect_path.end());
        wp_types.back() = WaypointType::DETOUR;
        std::vector<WaypointType> tmp;
        tmp.resize(connect_path.size(), WaypointType::DETOUR);
        wp_types.insert(wp_types.end(), tmp.begin(), tmp.end());
      }
    }

    ret_path.push_back(current_start);
    ret_path.push_back(current_end);
    wp_types.push_back(WaypointType::NORMAL_START);
    wp_types.push_back(WaypointType::NORMAL_END);

    previous_end       = current_end;
    previous_end_state = true;

    from_up = !from_up;
  }

  paths_length_.push_back(std::pair<double, double>(total_length, total_connect_length));
  waypoint_types_.push_back(wp_types);

  return ret_path;
}

void Cell::GetPaths(Work &work, bool simplified)
{
  if (tracks_.size() == 1)
  {
    sorted_tracks_ = tracks_;
    paths_         = {GetPaths(work, true, true, simplified), GetPaths(work, true, false, simplified)};

    return;
  }

  paths_ = {GetPaths(work, true, true, simplified), GetPaths(work, true, false, simplified),
            GetPaths(work, false, true, simplified), GetPaths(work, false, false, simplified)};

  return;
}