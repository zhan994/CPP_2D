#include "geometry/string_2.h"

std::string String_2::PointToString(const Point &pt)
{
  return std::to_string(pt.x()) + ":" + std::to_string(pt.y());
}

std::string String_2::SegmentToString(const Segment &seg)
{
  return PointToString(seg.first) + "_" + PointToString(seg.second);
}