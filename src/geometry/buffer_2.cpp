#include "geometry/buffer_2.h"
#include <iostream>

Segment Buffer_2::Shrink(const Segment &seg)
{
  Point src = seg.first;
  Point tgt = seg.second;

  double dx         = tgt.x() - src.x();
  double dy         = tgt.y() - src.y();
  double shrink_len = 1e-3;
  double len        = bg::distance(src, tgt);
  double shrink_dx  = shrink_len * dx / len;
  double shrink_dy  = shrink_len * dy / len;

  Point new_src(src.x() + shrink_dx, src.y() + shrink_dy);
  Point new_tgt(tgt.x() - shrink_dx, tgt.y() - shrink_dy);

  return Segment(new_src, new_tgt);
}

bool Buffer_2::Shrink(const Polygon &pg, Polygon &out)
{
  bg::strategy::buffer::join_miter                 join_strategy;
  bg::strategy::buffer::distance_symmetric<double> distance_strategy(-1e-3);
  bg::strategy::buffer::end_flat                   end_strategy;
  bg::strategy::buffer::side_straight              side_strategy;
  bg::strategy::buffer::point_circle               point_strategy;

  // note: <fix> empty buffer
  MultiPolygon ret;
  bg::buffer(pg, ret, distance_strategy, side_strategy, join_strategy, end_strategy, point_strategy);
  if (ret.empty())
  {
    out = pg;
  } else
  {
    out = ret.front();
  }

  return !ret.empty();
}

void Buffer_2::BBox(const Polygon &pg, std::vector<double> &bbox)
{
  Box ret;
  bg::envelope(pg, ret);
  Point min = ret.min_corner();
  Point max = ret.max_corner();

  bbox = {min.x(), min.y(), max.x(), max.y()};
}