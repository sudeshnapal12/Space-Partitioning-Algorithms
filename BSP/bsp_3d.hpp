#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <cmath>
#include <map>
#include <vector>
#include <cstdlib> 

#include "partition_params.hpp"


const std::string TAB = "\t";

static int FACTOR = 4;
static int GLOBAL_MAX_LEVEL = 10000000;

class SpatialObject {
	public:
		double low[3];
		double high[3];
		SpatialObject(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z);
};

class BinarySplitNode {
	public:
		double low[3];
		double high[3];
		int level;
		bool isLeaf;
		bool canBeSplit;
		int size;
		BinarySplitNode* first;
		BinarySplitNode* second;
		std::vector<SpatialObject*> objectList;

		BinarySplitNode(double min_x, double min_y, double min_z, double max_x, 
				double max_y, double max_z, int level);

		~BinarySplitNode();

		bool addObject(SpatialObject *object);
		bool intersects(SpatialObject *object);
		bool addObjectIgnore(SpatialObject *object);    
};

// Global variables (used by BinarySplitNode)
std::vector<BinarySplitNode*> leafNodeList;
std::vector<SpatialObject*> listAllObjects;
int bucket_size;
long total_count = 0;

string prefix_tile_id = "BSP";

//function defs

void process_input(struct partition_op &partop);

bool read_input(struct partition_op & partop, char** argv);
void update_partop_info(struct partition_op & partop, 
	string uppertileid, string newprefix);

 #include "BinarySplitNode.hpp"
