/**
 * \file planning_2.h
 * \author Zhihao Zhan (zhanzhihao_dt@163.com)
 * \brief 航线规划接口
 * \version 0.1
 * \date 2023-06-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef INTERFACE_PLAN_2_H
#define INTERFACE_PLAN_2_H

#include "work/route_2.h"
#include "planning.pb.h"

/**
 * \brief 设置地块
 *
 * \param outline
 * \param holes
 * \param start_pt
 * \param rotation
 * \param interval
 * \param offset
 * \param preview
 * \return std::shared_ptr<Route_2>
 */
std::shared_ptr<Route_2> Set_2(const std::vector<Point> &             outline,
                               const std::vector<std::vector<Point>> &holes,
                               const Point &                          start_pt,
                               double                                 rotation,
                               double                                 interval,
                               double                                 offset,
                               bool                                   preview);

/**
 * \brief 设置地块
 *
 * \param data
 * \param size
 * \return std::shared_ptr<Route_2>
 */
std::shared_ptr<Route_2> Set_2(const void *data, int size);

/**
 * \brief 航线规划
 *
 * \param route_ptr
 * \param path
 * \param types
 * \param line_nums
 */
void Plan_2(std::shared_ptr<Route_2>   route_ptr,
            std::vector<Point> &       path,
            std::vector<WaypointType> &types,
            std::vector<int> &         line_nums);

/**
 * \brief
 *
 * \param route_ptr
 * \return protocol::apollo::planning::V1::RoutePointsPlanned
 */
protocol::apollo::planning::V1::RoutePointsPlanned Plan_2(std::shared_ptr<Route_2> route_ptr);

/**
 * \brief 终止规划
 *
 * \param route_ptr
 */
void Stop_2(std::shared_ptr<Route_2> route_ptr);

/**
 * \brief 航线规划
 *
 * \param outline
 * \param holes
 * \param start_pt
 * \param rotation
 * \param interval
 * \param offset
 * \param preview
 * \param path
 * \param types
 * \param line_nums
 */
void Plan_2(const std::vector<Point> &             outline,
            const std::vector<std::vector<Point>> &holes,
            const Point &                          start_pt,
            double                                 rotation,
            double                                 interval,
            double                                 offset,
            bool                                   preview,
            std::vector<Point> &                   path,
            std::vector<WaypointType> &            types,
            std::vector<int> &                     line_nums);

/**
 * \brief pb接口
 *
 * \param param
 * \return std::string
 */
std::string Plan_2(const std::string &param);

/**
 * \brief 字节流pb接口
 *
 * \param data 字节流
 * \param size 字节流大小
 * \return protocol::apollo::planning::V1::RoutePointsPlanned
 */
protocol::apollo::planning::V1::RoutePointsPlanned Plan_2(const void *data, int size);

#endif // INTERFACE_PLAN_2_H