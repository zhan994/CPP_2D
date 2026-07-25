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
add_library(vis SHARED 
  src/visualization/vis.cpp
)

target_link_libraries(vis 
  ${Boost_LIBRARIES}
)

set_target_properties(vis 
  PROPERTIES 
  VERSION 1.4
  SOVERSION 1
)


###################### PLAN #######################
add_library(cpp_2d SHARED 
  ${WORK_SRC}
  ${GEOM_UTILITY_SRC}
)

# protobuf need pthread
target_link_libraries(cpp_2d
  ${Boost_LIBRARIES}
  pthread 
)

set_target_properties(cpp_2d 
  PROPERTIES 
  VERSION 1.4
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
  vis
  cpp_2d
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
# test geom.tf
add_executable(test_geometry_tf
  test/geometry/test_tf.cpp
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_geometry_tf
  ${Boost_LIBRARIES}
)


# test geom.buffer
add_executable(test_geometry_buffer
  test/geometry/test_buffer.cpp
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_geometry_buffer
  ${Boost_LIBRARIES}
  vis
)

# test geom.bool
add_executable(test_geometry_bool
  test/geometry/test_bool.cpp
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_geometry_bool
  ${Boost_LIBRARIES}
  vis
)

# test geom.utility
add_executable(test_geometry_utility
  test/geometry/test_utility.cpp
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_geometry_utility
  ${Boost_LIBRARIES}
  vis
)

# test work.work
add_executable(test_work_work
  test/work/test_work.cpp
  ${WORK_SRC}
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_work_work
  ${Boost_LIBRARIES}
  vis
)


# test work.cpp_2d
add_executable(test_work_cpp_2d
  test/work/test_cpp_2d.cpp
  ${WORK_SRC}
  ${GEOM_UTILITY_SRC}
)

target_link_libraries(test_work_cpp_2d
  ${Boost_LIBRARIES}
  vis
)