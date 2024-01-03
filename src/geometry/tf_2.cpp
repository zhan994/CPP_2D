#include "geometry/tf_2.h"

Point TF_2::Rotate(const Point &pt, const Point &center, double angle)
{
  Point pt0, pt1, pt2;

  bg::strategy::transform::translate_transformer<double, 2, 2> t1(-center.x(), -center.y());
  bg::transform(pt, pt0, t1);

  // note: clockwise
  bg::strategy::transform::rotate_transformer<bg::degree, double, 2, 2> r(-angle);
  bg::transform(pt0, pt1, r);

  bg::strategy::transform::translate_transformer<double, 2, 2> t2(center.x(), center.y());
  bg::transform(pt1, pt2, t2);

  return pt2;
}

Segment TF_2::Rotate(const Segment &seg, const Point &center, double angle)
{
  Segment seg0, seg1, seg2;

  bg::strategy::transform::translate_transformer<double, 2, 2> t1(-center.x(), -center.y());
  bg::transform(seg, seg0, t1);

  // note: clockwise
  bg::strategy::transform::rotate_transformer<bg::degree, double, 2, 2> r(-angle);
  bg::transform(seg0, seg1, r);

  bg::strategy::transform::translate_transformer<double, 2, 2> t2(center.x(), center.y());
  bg::transform(seg1, seg2, t2);

  return seg2;
}

Polygon TF_2::Rotate(const Polygon &pg, const Point &center, double angle)
{
  Polygon pg0, pg1, pg2;

  bg::strategy::transform::translate_transformer<double, 2, 2> t1(-center.x(), -center.y());
  bg::transform(pg, pg0, t1);

  // note: clockwise
  bg::strategy::transform::rotate_transformer<bg::degree, double, 2, 2> r(-angle);
  bg::transform(pg0, pg1, r);

  bg::strategy::transform::translate_transformer<double, 2, 2> t2(center.x(), center.y());
  bg::transform(pg1, pg2, t2);

  return pg2;
}