/**
 * \file bg.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief boost.geometry
 * \version 0.1
 * \date 2023-07-10
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef GEOMETRY_BG_H
#define GEOMETRY_BG_H

#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/multi_polygon.hpp>

namespace bg = boost::geometry;
typedef bg::model::d2::point_xy<double>   Point;
typedef bg::model::segment<Point>         Segment;
typedef bg::model::linestring<Point>      LineString;
typedef bg::model::box<Point>             Box;
typedef bg::model::polygon<Point>         Polygon;
typedef bg::model::multi_polygon<Polygon> MultiPolygon;

#endif // GEOMETRY_BG_H