cmake_minimum_required(VERSION 2.8)
project(planning_cpp)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++14")

find_package(Boost REQUIRED COMPONENTS filesystem)

include_directories(
  include
  ${Boost_INCLUDE_DIRS}
)

aux_source_directory(src/geometry GEOM_UTILITY_SRC)

aux_source_directory(src/work WORK_SRC)


###################### VIS #######################
add_library(vis_2 SHARED 
  src/visualization/vis_2.cpp
)

target_link_libraries(vis_2 
  ${Boost_LIBRARIES}
)

set_target_properties(vis_2 
  PROPERTIES 
  VERSION 1.3 
  SOVERSION 1
)


###################### PLAN #######################
add_library(planning_2 SHARED 
  ${WORK_SRC}
  ${GEOM_UTILITY_SRC}
)

# protobuf need pthread
target_link_libraries(planning_2
  ${Boost_LIBRARIES}
  pthread 
)

set_target_properties(planning_2 
  PROPERTIES 
  VERSION 1.3 
  SOVERSION 1
)


###################### INSTALL #######################
set(INSTALL_DIR ${PROJECT_SOURCE_DIR}/install)
if(EXISTS ${INSTALL_DIR} AND IS_DIRECTORY ${INSTALL_DIR})
  file(REMOVE_RECURSE ${INSTALL_DIR})
endif()

if(EXISTS ${PROJECT_SOURCE_DIR}/lib AND IS_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
  file(REMOVE_RECURSE ${PROJECT_SOURCE_DIR}/lib)
endif()

install(TARGETS
  vis_2
  planning_2
  DESTINATION ${PROJECT_SOURCE_DIR}/lib
)

install(DIRECTORY 
  include
  DESTINATION ${INSTALL_DIR}
)

install(DIRECTORY 
  lib
  DESTINATION ${INSTALL_DIR}
)

###################### TEST #######################
# test geom.tf_2
add_executable(test_geometry_tf_2
  test/geometry/test_tf_2.cpp
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_geometry_tf_2
  ${Boost_LIBRARIES}
)


# test geom.buffer_2
add_executable(test_geometry_buffer_2
  test/geometry/test_buffer_2.cpp
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_geometry_buffer_2
  ${Boost_LIBRARIES}
  vis_2
)

# test geom.bool_2
add_executable(test_geometry_bool_2
  test/geometry/test_bool_2.cpp
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_geometry_bool_2
  ${Boost_LIBRARIES}
  vis_2
)

# test geom.utility_2
add_executable(test_geometry_utility_2
  test/geometry/test_utility_2.cpp
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_geometry_utility_2
  ${Boost_LIBRARIES}
  vis_2
)

# test work.work_2
add_executable(test_work_work_2
  test/work/test_work_2.cpp
  ${WORK_SRC}
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_work_work_2
  ${Boost_LIBRARIES}
  vis_2
)


# test work.route_2
add_executable(test_work_route_2
  test/work/test_route_2.cpp
  ${WORK_SRC}
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_work_route_2
  ${Boost_LIBRARIES}
  vis_2
)