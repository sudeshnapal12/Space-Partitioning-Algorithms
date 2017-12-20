#include <iostream>
//#include <cstdlib>
//#include <string>
//#include <cstring>
//#include <sstream>
//#include <fstream>
//#include <cstring>
//#include <cmath>
//#include <map>
//#include <unordered_map>
//#include <vector>
#include <spatialindex/SpatialIndex.h>
#include "partition_params.hpp"

#ifdef DEBUGAREA
#include <utilities/tile_rectangle.h>
#endif

const std::string TAB = "\t";

/* Sort-tile recursive (STR) partitioning method */

vector<SpatialIndex::RTree::Data*> vec;

int indexCapacity = 20;
double fillFactor = 0.9999;
string prefix_tile_id = "STR";

#ifdef DEBUGAREA
vector<Rectangle*> list_rec;
#endif

