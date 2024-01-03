# coverage_planning_2d

**A repo. which maintains package about 2d coverage path planning for uav.**

## Dependencies

1. `boost-1.71.0.0ubuntu2`

   ```bash
   sudo apt install libboost-dev
   ```

2. `gnu-plot`

   ```bash
   sudo apt install gnuplot
   ```

3. `protobuf-3.4.0`

## Build

```bash
mkdir build && cd build
cmake ..
make -j

# test
./test_interface_planning_2
```

## Install

If you want to get generated headers and libraries, refer commands below and you can copy directory named `install`.

```bash
make install
copy ../install tgt_path
```

## Example

Refer to code `test/interface/test_planning_2.cpp`.

```cpp
#include "interface/planning_2.h"

int main()
{
  std::vector<Point> outline = 
  	{Point(13220710.834, 3760418.741), Point(13220806.594, 3760418.79),
     Point(13220801.438, 3760359.008), Point(13220702.703, 3760360.44)};
  std::vector<std::vector<Point>> holes = 
  {{Point(13220779.412, 3760402.492), Point(13220786.001, 3760359.443),
    Point(13220781.39, 3760356.619), Point(13220760.697, 3760371.512),
    Point(13220760.227, 3760373.227), Point(13220773.367, 3760403.283)}};
  Point start_pt(13220709.655, 3760420.094);

  double rotation = 0.;
  double interval = 2;
  double offset   = 0.3;
  bool preview = true;
  std::vector<Point>        path;
  std::vector<WaypointType> types;
  std::vector<int>          line_nums;
  Plan_2(outline, holes, start_pt, rotation, interval, offset, preview, path, types, line_nums);
    
  return 0;
}
```

