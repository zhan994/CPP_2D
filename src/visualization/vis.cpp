#include "visualization/vis.h"

Vis::Vis(std::string prefix) : prefix_(prefix)
{
  if (!IsDirExist(prefix_))
  {
    if (!CreateNewDir(prefix_))
    {
      std::cerr << "Failed to create " << prefix_ << std::endl;
      return;
    }
  }
}

bool Vis::IsDirExist(const std::string &path_name)
{
  return boost::filesystem::exists(path_name) && boost::filesystem::is_directory(path_name);
}

bool Vis::CreateNewDir(const std::string &path_name)
{
  return boost::filesystem::create_directories(path_name);
}

bool Vis::IsFileExist(const std::string &file_name)
{
  return boost::filesystem::exists(file_name) && boost::filesystem::is_regular_file(file_name);
}

void Vis::NewFile(std::string label, const Polygon &data, std::string color)
{
  std::string   data_file_name = prefix_ + "/" + label + "_" + "polygon" + ".data";
  std::ofstream data_file;

  if (IsFileExist(data_file_name))
  {
    data_file.open(data_file_name, std::ofstream::app);
  } else
  {
    data_file.open(data_file_name);
  }

  if (!std::count(polygon_fn_.begin(), polygon_fn_.end(), label))
  {
    polygon_fn_.push_back(label);
    polygon_color_.push_back(color);
  }

  if (!data_file.is_open())
  {
    std::cerr << "Failed to open polygon data file." << std::endl;
    return;
  }

  for (const auto &pt : data.outer()) data_file << pt.x() << " " << pt.y() << std::endl;
  data_file << std::endl;

  for (const auto &hole : data.inners())
  {
    for (const auto &hole_pt : hole) data_file << hole_pt.x() << " " << hole_pt.y() << std::endl;
    data_file << std::endl;
  }

  data_file.close();
}

void Vis::NewFile(std::string label, const std::vector<Polygon> &data, std::string color)
{
  std::string   data_file_name = prefix_ + "/" + label + "_" + "holes" + ".data";
  std::ofstream data_file;

  if (IsFileExist(data_file_name))
  {
    data_file.open(data_file_name, std::ofstream::app);
  } else
  {
    data_file.open(data_file_name);
  }

  if (!std::count(hole_fn_.begin(), hole_fn_.end(), label))
  {
    hole_fn_.push_back(label);
    hole_color_.push_back(color);
  }

  if (!data_file.is_open())
  {
    std::cerr << "Failed to open polygon data file." << std::endl;
    return;
  }

  for (const auto &pg : data)
  {
    for (const auto &pt : pg.outer()) data_file << pt.x() << " " << pt.y() << std::endl;
    data_file << std::endl;

    for (const auto &hole : pg.inners())
    {
      for (const auto &hole_pt : hole) data_file << hole_pt.x() << " " << hole_pt.y() << std::endl;
      data_file << std::endl;
    }
  }

  data_file.close();
}

void Vis::NewFile(std::string label, const std::vector<Segment> &data, std::string color)
{
  std::string   data_file_name = prefix_ + "/" + label + "_" + "segment" + ".data";
  std::ofstream data_file;

  if (IsFileExist(data_file_name))
  {
    data_file.open(data_file_name, std::ofstream::app);
  } else
  {
    data_file.open(data_file_name);
  }

  if (!std::count(segment_fn_.begin(), segment_fn_.end(), label))
  {
    segment_fn_.push_back(label);
    segment_color_.push_back(color);
  }

  if (!data_file.is_open())
  {
    std::cerr << "Failed to open segment data file." << std::endl;
    return;
  }

  for (const auto &seg : data)
  {
    data_file << seg.first.x() << " " << seg.first.y() << std::endl;
    data_file << seg.second.x() << " " << seg.second.y() << std::endl;
    data_file << std::endl;
  }

  data_file.close();
}

void Vis::NewFile(std::string label, const std::vector<Point> &data, std::string color)
{
  std::string data_file_name = prefix_ + "/" + label + "_" + "point" + ".data";

  std::ofstream data_file;

  if (IsFileExist(data_file_name))
  {
    data_file.open(data_file_name, std::ofstream::app);
  } else
  {
    data_file.open(data_file_name);
  }

  if (!std::count(point_fn_.begin(), point_fn_.end(), label))
  {
    point_fn_.push_back(label);
    point_color_.push_back(color);
  }

  if (!data_file.is_open())
  {
    std::cerr << "Failed to open point data file." << std::endl;
    return;
  }

  for (const auto &pt : data)
  {
    data_file << pt.x() << " " << pt.y() << std::endl;
  }

  data_file.close();
}

void Vis::Show()
{
  std::string   gnu_plot_script_name = prefix_ + '/' + "plot.gnu";
  std::ofstream gnu_plot_script(gnu_plot_script_name);
  if (!gnu_plot_script.is_open())
  {
    std::cerr << "Failed to open the gnu plot script file." << std::endl;
    return;
  }

  gnu_plot_script << "set key top outside horizontal center" << std::endl;

  bool is_start_show = true;

  if (!polygon_fn_.empty())
  {
    for (int i = 0; i < polygon_fn_.size(); i++)
    {
      std::string data_file_name = prefix_ + "/" + polygon_fn_[i] + "_" + "polygon" + ".data";
      std::string polygon_script;
      std::string color = polygon_color_[i];

      if (is_start_show)
      {
        is_start_show  = false;
        polygon_script = "plot '" + data_file_name + "' with line linewidth 0.5 lc rgb '" + color + "' title'" +
                         polygon_fn_[i] + "'";
      } else
      {
        polygon_script =
            ",'" + data_file_name + "' with line linewidth 0.5 lc rgb '" + color + "' title'" + polygon_fn_[i] + "'";
      }

      gnu_plot_script << polygon_script;
    }
  }

  if (!hole_fn_.empty())
  {
    for (int i = 0; i < hole_fn_.size(); i++)
    {
      std::string data_file_hole_name = prefix_ + "/" + hole_fn_[i] + "_" + "holes" + ".data";
      std::string hole_script;
      std::string color = hole_color_[i];

      if (is_start_show)
      {
        is_start_show = false;
        hole_script   = "plot '" + data_file_hole_name + "' with line linewidth 0.5 lc rgb '" + color + "' title'" +
                      hole_fn_[i] + "'";
      } else
      {
        hole_script =
            ",'" + data_file_hole_name + "' with line linewidth 0.5 lc rgb '" + color + "' title'" + hole_fn_[i] + "'";
      }

      gnu_plot_script << hole_script;
    }
  }

  if (!point_fn_.empty())
  {
    for (size_t i = 0; i < point_fn_.size(); ++i)
    {
      std::string data_file_name = prefix_ + "/" + point_fn_[i] + "_" + "point" + ".data";
      std::string point_script;
      std::string color = point_color_[i];

      if (is_start_show)
      {
        is_start_show = false;
        point_script =
            "plot '" + data_file_name + "' with point pointsize 0.5 lc rgb '" + color + "' title'" + point_fn_[i] + "'";
      } else
      {
        point_script =
            ",'" + data_file_name + "' with point pointsize 0.5 lc rgb '" + color + "' title'" + point_fn_[i] + "'";
      }

      gnu_plot_script << point_script;
    }
  }

  if (!segment_fn_.empty())
  {
    for (int i = 0; i < segment_fn_.size(); i++)
    {
      std::string data_file_name = prefix_ + "/" + segment_fn_[i] + "_" + "segment" + ".data";
      std::string segment_script;
      std::string color = segment_color_[i];

      if (is_start_show)
      {
        is_start_show  = false;
        segment_script = "plot '" + data_file_name + "' with line linewidth 0.5 lc rgb '" + color + "' title'" +
                         segment_fn_[i] + "'";
      } else
      {
        segment_script =
            ",'" + data_file_name + "' with line linewidth 0.5 lc rgb '" + color + "' title'" + segment_fn_[i] + "'";
      }

      gnu_plot_script << segment_script;
    }
  }

  gnu_plot_script.close();

  std::string command = "gnuplot -p " + gnu_plot_script_name;
  system(command.c_str());
}

void Vis::Clear()
{
  polygon_fn_.clear();
  polygon_color_.clear();
  hole_fn_.clear();
  hole_color_.clear();
  segment_fn_.clear();
  segment_color_.clear();
  point_fn_.clear();
  point_fn_.clear();

  std::string command = "cd " + prefix_ + "&& rm *.data";
  system(command.c_str());
}