^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for package planning_cpp
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

1.0.0 (2023-06-14)
------------------
* First unstable development release

1.1.0 (2023-06-15)
------------------
* Fix bug: result of "Buffer_2::Shrink Polygon" is empty 
* Add "Utility::DRound" interface to control double precision
* Optimize dijkstra search graph due to "remove_vertex"
* Add "planning.proto" interface
* Fix bug: protobuf needs pthread support to avoid "std::system_error"

1.2.0 (2023-06-29)
------------------
* Fix bug: "std::sort" need serious comp. function
* Fix bug: pb interface avoid local memory without deleting

1.3.0 (2023-07-05)
------------------
* Optimize time cost due to complex areas
* Optimize the method about how to choose the start cell 
* Free redundant time cost due to reprecessing initial data