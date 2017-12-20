#include "bsp_3d.hpp"

using namespace std;

int main(int argc, char **argv) {
	cout.precision(15);
	struct partition_op partop;
	if (!extract_params_partitioning(argc, argv, partop)) {
		#ifdef DEBUG
		cerr << "Fail to extract parameters" << endl;
		#endif
		return -1;
	}
	bucket_size = partop.bucket_size;   
	if (!read_input(partop, argv)) {
		cerr << "Error reading input in" << endl;
		return -1;
	}
	cerr << total_count << endl;
	return 0;
}

bool read_input(struct partition_op &partop, char *argv[]) {
	string input_line;
	string prevtileid = "";
	string tile_id;
	vector<string> fields;
	double low[3];
	double high[3];

	partop.object_count = 0;
	//while (cin && getline(cin, input_line) && !cin.eof()) {
	ifstream file(argv[1]);
	while (file && getline(file, input_line)) {
		try {
			istringstream ss(input_line);
			ss >> tile_id >> low[0] >> low[1] >> low[2] >>  high[0] >> high[1] >> high[2];
			
			//cout << "input_line" << input_line << "\n";
			//cout << "tile_id  " << tile_id << "\n";
			//cout << "low[0] " << low[0] << "\n";
			//cout << "high[0] "<< high[0] << "\n";
			//cout << "low[1] " << low[1] << "\n";
			//cout << "high[1] " << high[1] << "\n";
			//cout << "low[2] " << low[2] << "\n";
			//cout << "high[2] " << high[2] << "\n";
			//cout << "prevtileid " << prevtileid << "\n";

			if (prevtileid.compare(tile_id) != 0 && prevtileid.size() > 0) {
				cout << "sud1 \n";
				update_partop_info(partop, prevtileid, prevtileid  + prefix_tile_id);
				process_input(partop);
				// total_count += partop.object_count;
				partop.object_count = 0;
			}
			prevtileid = tile_id;
			// Create objects
			SpatialObject *obj = new SpatialObject(low[0], low[1], low[2], high[0], high[1], high[2]);

			listAllObjects.push_back(obj);

			fields.clear();
			partop.object_count++;
		} catch (...) {

		}
	}
	if (partop.object_count > 0) {
		//cout << "sud2 \n";
		// Process last tile
		if (partop.region_mbbs.size() == 1) {
			//cout << "sud3 \n";
			update_partop_info(partop, prevtileid, prefix_tile_id);
		} else {
			//cout << "sud4 \n";
			// First level of partitioning
			update_partop_info(partop, prevtileid, prevtileid + prefix_tile_id);
		}
		process_input(partop);
	}
	//total_count += partop.object_count;
	cleanup(partop);
	return true;
}

void process_input(struct partition_op &partop) {
	BinarySplitNode *tree = new BinarySplitNode(partop.low[0], 
		partop.low[1], partop.low[2], partop.high[0], partop.high[1], partop.high[2], 0);
	cout << "leaf nodes " << TAB << partop.low[0] << TAB << partop.low[1] << TAB << partop.low[2] << TAB <<
		partop.high[0] << TAB << partop.high[1] << TAB << partop.high[2] << "\n";
	leafNodeList.push_back(tree);
	for (vector<SpatialObject*>::iterator it = listAllObjects.begin(); it != listAllObjects.end(); it++) {
		tree->addObject(*it);
	}

	int tid = 0; //bucket id
	//int countLeaf = 0;
	for(vector<BinarySplitNode*>::iterator it = leafNodeList.begin(); it != leafNodeList.end(); it++ ) {
		BinarySplitNode *tmp = *it;
		if (tmp->isLeaf) {
			/* Composite key */
			cout << partop.prefix_tile_id << tid 
				<< TAB << tmp->low[0] << TAB << tmp->low[1] << TAB << tmp->low[2]
				<< TAB << tmp->high[0] << TAB << tmp->high[1] << TAB << tmp->high[2]
				#ifdef DEBUG
				<< TAB << tmp->size 
				#endif
				<< endl;
			tid++;
		}
	}

	// Memory cleanup here. 

	for (vector<SpatialObject*>::iterator it = listAllObjects.begin(); it != listAllObjects.end(); it++) { 
		delete *it;
	}

	for(vector<BinarySplitNode*>::iterator it = leafNodeList.begin(); it != leafNodeList.end(); it++ ) {
	//	(*it)->objectList.clear();
		delete *it;

	}
	listAllObjects.clear(); 
	leafNodeList.clear();
	//delete tree;
}

