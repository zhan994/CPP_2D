cmake_minimum_required(VERSION 2.8)
project(planning_cpp)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_FLAGS "-std=c++14")

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(Protobuf REQUIRED)

set(DST_DIR ${PROJECT_BINARY_DIR}/install/proto)
if(EXISTS ${DST_DIR} AND IS_DIRECTORY ${DST_DIR})
  SET(PROTO_DST_DIR ${DST_DIR})
else()
  file(MAKE_DIRECTORY ${DST_DIR})
  SET(PROTO_DST_DIR ${DST_DIR})
endif()

file(GLOB PROTO_FILES ${PROJECT_SOURCE_DIR}/include/proto/*.proto)
foreach(fp ${PROTO_FILES})
  message("proto_file_path: " ${fp})
  get_filename_component(fn ${fp} NAME)
  message("proto_file_name: " ${fn})
  execute_process(COMMAND 
    ${PROTOBUF_PROTOC_EXECUTABLE} 
    --cpp_out=${PROTO_DST_DIR} 
    --proto_path=${PROJECT_SOURCE_DIR}/include/proto
    ${fn}
  )
endforeach(fp)

include_directories(
  include
  ${PROTO_DST_DIR}
  ${Boost_INCLUDE_DIRS}
  ${PROTOBUF_INCLUDE_DIR}
)

aux_source_directory(src/geometry GEOM_UTILITY_SRC)

aux_source_directory(src/work WORK_SRC)

aux_source_directory(src/interface INTERFACE_SRC)

aux_source_directory(${PROTO_DST_DIR} PROTO_SRC) 

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
  ${INTERFACE_SRC}
  ${PROTO_SRC}
  ${WORK_SRC}
  ${GEOM_UTILITY_SRC}
)

# protobuf need pthread
target_link_libraries(planning_2
  ${Boost_LIBRARIES}
  ${PROTOBUF_LIBRARY}
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

file(GLOB PROTO_HEADERS ${PROTO_DST_DIR}/*.h)
install(FILES 
  ${PROTO_HEADERS}
  DESTINATION ${INSTALL_DIR}/include/interface
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

# test interface.planning_2
add_executable(test_interface_planning_2
  test/interface/test_planning_2.cpp
)

target_link_libraries(test_interface_planning_2
  planning_2
  vis_2
)

# test interface.planning_2_pb
add_executable(test_interface_planning_2_pb
  test/interface/test_planning_2_pb.cpp
)

target_link_libraries(test_interface_planning_2_pb
  planning_2
  vis_2
)

# test interface.planning_2_pb_stop
add_executable(test_interface_planning_2_pb_stop
  test/interface/test_planning_2_pb_stop.cpp
)

target_link_libraries(test_interface_planning_2_pb_stop
  planning_2
  vis_2
)
