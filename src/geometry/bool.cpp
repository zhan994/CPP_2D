#include "geometry/bool.h"

void Bool::Intersection(const Segment &seg1, const Segment &seg2, std::vector<Point> &out)
{
  out.clear();
  bg::intersection(seg1, seg2, out);
}

void Bool::Intersection(const Segment &seg, const Polygon &pg, std::vector<Point> &out)
{
  out.clear();
  LineString ls, res;
  ls.push_back(seg.first);
  ls.push_back(seg.second);
  bg::intersection(ls, pg, res);
  for (const auto &pt : res) out.push_back(pt);
}

void Bool::Intersection(const Polygon &pg1, const Polygon &pg2, std::vector<Polygon> &out)
{
  out.clear();
  bg::intersection(pg1, pg2, out);
}

void Bool::Difference(const Polygon &pg1, const Polygon &pg2, std::vector<Polygon> &out)
{
  out.clear();
  bg::difference(pg1, pg2, out);
}

void Bool::Union(const Polygon &pg1, const Polygon &pg2, std::vector<Polygon> &out)
{
  out.clear();
  bg::union_(pg1, pg2, out);
}

bool Bool::CoveredBy(const Segment &seg, const Polygon &pg)
{
  LineString ls;
  ls.push_back(seg.first);
  ls.push_back(seg.second);

  return bg::covered_by(ls, pg);
}

bool Bool::Within(const Segment &seg, const Polygon &pg)
{
  LineString ls;
  ls.push_back(seg.first);
  ls.push_back(seg.second);

  return bg::within(ls, pg);
}