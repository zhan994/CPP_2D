#include "geometry/string.h"

std::string String::PointToString(const Point &pt)
{
  return std::to_string(pt.x()) + ":" + std::to_string(pt.y());
}

std::string String::SegmentToString(const Segment &seg)
{
  return PointToString(seg.first) + "_" + PointToString(seg.second);
}