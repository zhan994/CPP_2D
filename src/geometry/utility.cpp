#include "geometry/utility.h"

Segment Utility::MergeVerticalSegments(const Segment &seg1, const Segment &seg2)
{
  double x = seg1.first.x();

  std::vector<double> vec_y;
  vec_y.push_back(seg1.first.y());
  vec_y.push_back(seg1.second.y());
  vec_y.push_back(seg2.first.y());
  vec_y.push_back(seg2.second.y());

  double y_min = *std::min_element(vec_y.begin(), vec_y.end());
  double y_max = *std::max_element(vec_y.begin(), vec_y.end());

  return Segment(Point(x, y_min), Point(x, y_max));
}

bool Utility::CompPoint(const Point &pt1, const Point &pt2)
{
  double eps = 1e-6;

  double dx = pt1.x() - pt2.x();
  if (dx < -eps)
  {
    return true;
  } else if (dx > eps)
  {
    return false;
  }

  double dy = pt1.y() - pt2.y();
  if (dy < -eps)
  {
    return true;
  } else if (dy > eps)
  {
    return false;
  }

  // note: <fix> comp bug, must "<", not "<=""
  return false;
}

bool Utility::CompSegment(const Segment &seg1, const Segment &seg2)
{
  double eps = 1e-6;

  double s_dx = seg1.first.x() - seg2.first.x();
  if (s_dx < -eps)
  {
    return true;
  } else if (s_dx > eps)
  {
    return false;
  }

  double s_dy = seg1.first.y() - seg2.first.y();
  if (s_dy < -eps)
  {
    return true;
  } else if (s_dy > eps)
  {
    return false;
  }

  double e_dx = seg1.second.x() - seg2.second.x();
  if (e_dx < -eps)
  {
    return true;
  } else if (e_dx > eps)
  {
    return false;
  }

  double e_dy = seg1.second.y() - seg2.second.y();
  if (e_dy < -eps)
  {
    return true;
  } else if (e_dy > eps)
  {
    return false;
  }

  // note: <fix> comp bug, must "<", not "<=""
  return false;
}

Polygon Utility::SetData(const std::vector<Point> &data)
{
  Polygon ret;
  for (const auto &pt : data) ret.outer().push_back(pt);
  if (!bg::equals(data.front(), data.back()))
    ret.outer().push_back(data.front());
  bg::correct(ret);

  return ret;
}

double Utility::DRound(double dval, int n)
{
  char  l_fmtp[32], l_buf[64];
  char *p_str;
  sprintf(l_fmtp, "%%.%df", n);
  if (dval >= 0)
    sprintf(l_buf, l_fmtp, dval);
  else
    sprintf(l_buf, l_fmtp, dval);
  return ((double)strtod(l_buf, &p_str));
}