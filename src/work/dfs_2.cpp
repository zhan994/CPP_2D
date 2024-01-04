#include "work/dfs_2.h"

DFS_2::DFS_2()
{}

DFS_2::DFS_2(const Work_2 &work, const std::vector<Cell_2> &cells, int max_cost, int cells_threshold)
    : work_(work),
      cells_(cells),
      N_(cells.size()),
      max_cost_(max_cost),
      cells_threshold_(cells_threshold),
      cost_(0),
      best_solution_weight_(std::numeric_limits<double>::max())
{
  visited_.resize(N_, false);
  edge_weights_.clear();
  best_path_.clear();
}

int DFS_2::NumNodes(size_t ci)
{
  Cell_2 cell = cells_[ci];
  return cell.NumNodes();
}

double DFS_2::GetWeight(size_t ci1, size_t ni1, size_t ci2, size_t ni2)
{
  double ret;

  std::string key =
      std::to_string(ci1) + ":" + std::to_string(ni1) + "_" + std::to_string(ci2) + ":" + std::to_string(ni2);
  if (edge_weights_.find(key) == edge_weights_.end())
  {
    ret                = ComputeWeight(ci1, ni1, ci2, ni2);
    edge_weights_[key] = ret;
  } else
  {
    ret = edge_weights_[key];
  }

  return ret;
}

double DFS_2::ComputeWeight(size_t ci1, size_t ni1, size_t ci2, size_t ni2)
{
  std::vector<Point> paths_end1, paths_end2;
  cells_[ci1].GetPathEnd(ni1, paths_end1);
  cells_[ci2].GetPathEnd(ni2, paths_end2);
  Point node1_end   = paths_end1[1];
  Point node2_start = paths_end2[0];

  std::vector<Point> path;

  double path_len = 0.0;
  // note: <opti> time cost due to complex cells
  // note: add preview mode
  bool simplified = N_ > cells_threshold_;
  path_len = simplified ? bg::distance(node1_end, node2_start) : work_.ShortestPath(node1_end, node2_start, path);
  // std::cout << ci1 << " " << ni1 << ", " << ci2 << " " << ni2 << ", " << String_2::PointToString(node1_end) << " "
  //           << String_2::PointToString(node2_start) << ", " << path_len << std::endl;
  // std::cout << "Path: " << std::endl;
  // for (const auto &pt : path) std::cout << String_2::PointToString(pt) << std::endl;

  return path_len;
}

void DFS_2::FindMinimunPath(
    size_t sci, size_t sni, double interval, std::vector<Point> &path_out, std::vector<WaypointType> &wp_types_out)
{
  cost_                 = 0;
  best_solution_weight_ = std::numeric_limits<double>::max();
  visited_.resize(N_, false);
  visited_[sci] = true;

  std::pair<size_t, size_t>              scn_pair(sci, sni);
  std::vector<std::pair<size_t, size_t>> path = {scn_pair};
  DFS(sci, sni, path, 0);

  // std::cout << best_solution_weight_ << std::endl;
  // for (const auto &s : best_path_) std::cout << s.first << " " << s.second << std::endl;

  TranslatePath(interval, path_out, wp_types_out);
}

void DFS_2::DFS(size_t cur_ci, size_t cur_ni, std::vector<std::pair<size_t, size_t>> &path, double current_weight)
{
  if (cost_ >= max_cost_)
    return;

  if (path.size() == N_)
  {
    if (current_weight < best_solution_weight_)
    {
      best_solution_weight_ = current_weight;
      best_path_            = path;
    }

    return;
  }

  cost_++;
  std::vector<std::pair<std::pair<int, int>, double>> candi_edge_weights;
  for (size_t ci = 0; ci < N_; ++ci)
  {
    if (!visited_[ci])
    {
      for (size_t ni = 0; ni < NumNodes(ci); ++ni)
      {
        double                                 weight = GetWeight(cur_ci, cur_ni, ci, ni);
        std::pair<int, int>                    cni(ci, ni);
        std::pair<std::pair<int, int>, double> next_edge_weight(cni, weight);
        candi_edge_weights.push_back(next_edge_weight);
      }
    }
  }
  std::sort(candi_edge_weights.begin(), candi_edge_weights.end(),
            [](auto e1, auto e2) { return e1.second < e2.second; });

  // std::cout << "sort candi ================================= " << candi_edge_weights.size() << std::endl;
  // for (const auto &data : candi_edge_weights)
  //   std::cout << data.first.first << ", " << data.first.second << " " << data.second << std::endl;

  std::vector<std::pair<std::pair<int, int>, double>> select_edge_weights(candi_edge_weights.begin(),
                                                                          candi_edge_weights.begin() + 2);
  for (const auto &edge_weight : select_edge_weights)
  {
    double new_weight = current_weight + edge_weight.second;
    if (new_weight < best_solution_weight_)
    {
      size_t                    next_ci = edge_weight.first.first;
      size_t                    next_ni = edge_weight.first.second;
      std::pair<size_t, size_t> cn_pair(next_ci, next_ni);
      visited_[next_ci] = true;
      path.push_back(cn_pair);
      DFS(next_ci, next_ni, path, new_weight);
      path.pop_back();
      visited_[next_ci] = false;
    }
  }
}

void DFS_2::TranslatePath(double interval, std::vector<Point> &path_out, std::vector<WaypointType> &wp_types_out)
{
  double total_length = 0.0, total_connect_length = 0.0;
  path_out.clear();
  wp_types_out.clear();

  if (best_path_.empty())
    return;

  if (best_path_.size() == 1)
  {
    std::pair<size_t, size_t> cur_cni = best_path_.front();
    path_out                          = cells_[cur_cni.first].paths_[cur_cni.second];
    wp_types_out                      = cells_[cur_cni.first].waypoint_types_[cur_cni.second];
    total_length                      = cells_[cur_cni.first].paths_length_[cur_cni.second].first;
    total_connect_length              = cells_[cur_cni.first].paths_length_[cur_cni.second].second;
    std::cout << " ***************** Connection path ratio: " << total_connect_length / total_length
              << " ***************** " << std::endl;
    return;
  }

  for (size_t i = 0; i < best_path_.size() - 1; ++i)
  {
    std::pair<size_t, size_t> cur_cni    = best_path_[i];
    std::pair<size_t, size_t> next_cni   = best_path_[i + 1];
    std::vector<Point>        cur_path   = cells_[cur_cni.first].paths_[cur_cni.second];
    std::vector<Point>        next_path  = cells_[next_cni.first].paths_[next_cni.second];
    std::vector<WaypointType> cur_types  = cells_[cur_cni.first].waypoint_types_[cur_cni.second];
    std::vector<WaypointType> next_types = cells_[next_cni.first].waypoint_types_[next_cni.second];

    if (path_out.empty())
    {
      path_out.insert(path_out.end(), cur_path.begin(), cur_path.end());
      wp_types_out.insert(wp_types_out.end(), cur_types.begin(), cur_types.end());
      total_length += cells_[cur_cni.first].paths_length_[cur_cni.second].first;
      total_connect_length += cells_[cur_cni.first].paths_length_[cur_cni.second].second;
    }

    std::vector<Point> connect_path;
    double             connect_length = 0.0;

    connect_length = work_.ShortestPath(cur_path.back(), next_path.front(), connect_path);
    connect_path.assign(connect_path.begin() + 1, connect_path.end() - 1);
    path_out.insert(path_out.end(), connect_path.begin(), connect_path.end());
    path_out.insert(path_out.end(), next_path.begin(), next_path.end());

    std::string               cell_connection_type = GetCellConnectionType(cur_path, next_path, interval);
    std::vector<WaypointType> connect_types;
    if (cell_connection_type == "line_continue")
    {
      if (!connect_path.empty())
      {
        connect_types.resize(connect_path.size(), WaypointType::DETOUR);
        wp_types_out.back() = WaypointType::DETOUR;
      } else
      {
        std::cout << "Error: a track was unexpectedly segmented due to some reasons" << std::endl;
      }
    } else if (cell_connection_type == "turn")
    {
      if (!connect_path.empty())
      {
        connect_types.resize(connect_path.size(), WaypointType::DETOUR);
        wp_types_out.back() = WaypointType::DETOUR;
      }
    } else
    {
      if (!connect_path.empty())
      {
        connect_types.resize(connect_path.size(), WaypointType::DETOUR);
        wp_types_out.back() = WaypointType::DETOUR;
      } else
      {
        wp_types_out.back() = WaypointType::TRANSITION;
      }
    }

    wp_types_out.insert(wp_types_out.end(), connect_types.begin(), connect_types.end());
    wp_types_out.insert(wp_types_out.end(), next_types.begin(), next_types.end());
    total_length += (cells_[next_cni.first].paths_length_[next_cni.second].first + connect_length);
    total_connect_length += (cells_[next_cni.first].paths_length_[next_cni.second].second + connect_length);
  }

  std::cout << " total_length: " << total_length << ", total_connect_length: " << total_connect_length << std::endl;
  std::cout << " path size: " << path_out.size() << std::endl;
  std::cout << " ***************** Connection path ratio: " << total_connect_length / total_length
            << " ***************** " << std::endl;
  return;
}

std::string DFS_2::GetCellConnectionType(const std::vector<Point> &cur_path,
                                         const std::vector<Point> &next_path,
                                         double                    interval)
{
  Point p1 = cur_path[cur_path.size() - 2];
  Point p2 = cur_path[cur_path.size() - 1];
  Point p3 = next_path[0];
  Point p4 = next_path[1];

  if (std::abs(p1.x() - p3.x()) < 1e-6)
  {
    if (p1.y() > p2.y() && p2.y() > p3.y() && p3.y() > p4.y())
    {
      return "line_continue";
    } else if (p1.y() < p2.y() && p2.y() < p3.y() && p3.y() < p4.y())
    {
      return "line_continue";
    } else
    {
      return "transition";
    }
  }

  if (std::abs(std::abs(p1.x() - p3.x()) - interval) < 1e-6)
  {
    if (p1.y() > p2.y() && p3.y() < p4.y())
    {
      return "turn";
    } else if (p1.y() < p2.y() && p3.y() > p4.y())
    {
      return "turn";
    } else
    {
      return "transition";
    }
  }

  return "transition";
}