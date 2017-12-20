#include <map>

using namespace std;

/* Struct to hold MBB */
struct mbb_info {
	double low[3];
	double high[3];
};

/* Query operator */
struct partition_op {
	long bucket_size;
	int offset; // Field offset
	string prefix_tile_id;
	int object_count;

	/* Space span  */
	double low[3];
	double high[3];

	/* only at most one can be true  */
	bool to_be_normalized;
	bool to_be_denormalized;

	bool parallel_partitioning;
	string file_name;
	map<std::string, struct mbb_info *> region_mbbs;
};
