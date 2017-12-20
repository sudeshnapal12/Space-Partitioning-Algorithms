#include <cmath>
#include <algorithm>
#include <iostream>
#include <sstream>

SpatialObject::SpatialObject(double min_x, double min_y, double min_z, double max_x, double max_y, double max_z) {
	low[0] = min_x;
	low[1] = min_y,
	low[2] = min_z;
	high[0] = max_x;
	high[1] = max_y;
	high[2] = max_z;
}

BinarySplitNode::BinarySplitNode(double min_x, double min_y, double min_z,
		double max_x, double max_y, double max_z, int _level) {
	low[0] = min_x;
	low[1] = min_y,
	low[2] = min_z;
	high[0] = max_x;
	high[1] = max_y;
	high[2] = max_z;
	level = _level;
	isLeaf = true;
	canBeSplit = true;
	size = 0;
		//    leafNodeList.push_back(this);
}

BinarySplitNode::~BinarySplitNode() {
	objectList.clear();
}

bool BinarySplitNode::addObjectIgnore(SpatialObject *object) {
	size++;
	objectList.push_back(object);

	/*
	if (size > bucket_size) {
	   canBeSplit = false;
	}
	*/
	return true;
}

bool BinarySplitNode::addObject(SpatialObject *object) {
	size++;

	if (isLeaf) {
		/* Update the center */
		objectList.push_back(object);
		/*
		   if (size > bucket_size + 1) {
		   cerr << "WHY: Level " << level << endl;
		   }
		   */

		if (size > bucket_size && canBeSplit) {
			//cout << "sud bucket_size " << bucket_size << "\n";
			//if (level >= GLOBAL_MAX_LEVEL) {
				//canBeSplit = false;
				//return false;
			//}
			#ifdef DEBUG
	                cerr << "Splitting" << endl;
			#endif
			// Replaced by code below		
			// double *xList = new double[size];
			// double *yList = new double[size];

			/* Temporary variables */
			int tempCount = 0;
			double median[3];
			median[0] = median[1] = median[2] = 0;
			double stdevX;
			double stdevY;
			double stdevZ;
			double tmp_x;
			double tmp_y;
			double tmp_z;
			vector<double> cordLists[3];

			for(vector<SpatialObject *>::iterator iter = objectList.begin(); 
					iter != objectList.end(); iter++ ) {
				tmp_x = ((*iter)->low[0] + (*iter)->high[0]) / 2;
				tmp_y = ((*iter)->low[1] + (*iter)->high[1]) / 2;
				tmp_z = ((*iter)->low[2] + (*iter)->high[2]) / 2;
				cordLists[0].push_back(tmp_x);
				cordLists[1].push_back(tmp_y);
				cordLists[2].push_back(tmp_z);
			}

			#ifdef DEBUG
			cerr << "Done inserting to vec: " << cordLists[0].size() << " " << cordLists[1].size() << endl;
			#endif

			sort(cordLists[0].begin(), cordLists[0].end());
			sort(cordLists[1].begin(), cordLists[1].end());
			sort(cordLists[2].begin(), cordLists[2].end());
			if (size % 2 == 0) {
				/* The following code is not safe - generating seg fault due to imprecision
				nth_element(cordLists[0].begin(), 
					cordLists[0].begin() + size / 2 - 1,
					 cordLists[0].end());
				nth_element(cordLists[0].begin(), 
					cordLists[0].begin() + size / 2,
					 cordLists[0].end());
				nth_element(cordLists[1].begin(), 
					cordLists[1].begin() + size / 2 - 1,
					 cordLists[0].end());
				nth_element(cordLists[0].begin(), 
					cordLists[1].begin() + size / 2,
					 cordLists[1].end());
				*/
				median[0] = (cordLists[0][size / 2] + cordLists[0][size / 2 - 1]) / 2;
				median[1] = (cordLists[1][size / 2] + cordLists[1][size / 2 - 1]) / 2;
				median[2] = (cordLists[2][size / 2] + cordLists[2][size / 2 - 1]) / 2;
			} else {
				// Odd number of elements
				/* The following code is not safe - generating seg fault due to imprecision
				nth_element(cordLists[0].begin(), 
					cordLists[0].begin() + size / 2,
					 cordLists[0].end());
				nth_element(cordLists[1].begin(), 
					cordLists[1].begin() + size / 2,
					 cordLists[1].end());
				*/
				median[0] = cordLists[0][size / 2];
				median[1] = cordLists[1][size / 2];
				median[2] = cordLists[2][size / 2];
			}
			#ifdef DEBUG
			cerr << "Median x: " << median[0] << TAB << " median y: " << median[1] << TAB << " median z: " << median[2] << endl;
			#endif

			for (int k = 0; k < 3 ; k++) {
				if (median[k] <= low[k]) {
					median[k] = (FACTOR * low[k] + high[k]) / (FACTOR + 1);
				} else if (median[k] >= high[k]) {
					median[k] = (low[k] + FACTOR * high[k]) / (FACTOR + 1);
				}
			}

			cordLists[0].clear();
			cordLists[1].clear();
			cordLists[2].clear();
			/* Choose which direction to split */
			// Here we heuristically select the direction generating partitions 
			//   with more similar areas
			
			#ifndef BSP_BETA
			if ( (high[0] - low[0] > high[1] - low[1])
			&& (high[0] - low[0] > high[2] - low[2])) {
				// True == splitting along x generates a more balanced partition
				first = new BinarySplitNode(low[0], low[1], low[2], median[0], high[1], high[2], level + 1);
				second = new BinarySplitNode(median[0], low[1], low[2], high[0], high[1], high[2], level + 1);
			
			} else if((high[1] - low[1] > high[2] - low[2])
			&& (high[1] - low[1] > high[0] - low[0])){
				// True == splitting along y generates a more balanced partition
				first = new BinarySplitNode(low[0], low[1], low[2], high[0], median[1], high[2], level + 1);
				second = new BinarySplitNode(low[0], median[1], low[2], high[0], high[1], high[2], level + 1);
			} else {
				first = new BinarySplitNode(low[0], low[1], low[2], high[0], high[1], median[2], level + 1);
				second = new BinarySplitNode(low[0], low[1], median[2], high[0], high[1], high[2], level + 1);
			}

			#else
			if ( (high[0] - median[0]) * (median[0] - low[0]) / pow(high[0] - low[0], 2)
			 >  (high[1] - median[1]) * (median[1] - low[1]) / pow(high[1] - low[1], 2)
			 && (high[0] - median[0]) * (median[0] - low[0]) / pow(high[0] - low[0], 2) 
			 > (high[2] - median[2]) * (median[2] - low[2]) / pow(high[2] - low[2], 2)) {
				// True == splitting along x generates a more balanced partition
				first = new BinarySplitNode(low[0], low[1], low[2], median[0], high[1], high[2], level + 1);
				second = new BinarySplitNode(median[0], low[1], low[2], high[0], high[1], high[2], level + 1);
			
			} else if ((high[1] - median[1]) * (median[1]] - low[1]) / pow(high[1] - low[1], 2)
			> (high[0] - median[0]) * (median[0] - low[0]) / pow(high[0] - low[0], 2)
			&& (high[1] - median[1]) * (median[1] - low[1]) / pow(high[1] - low[1], 2)
			> (high[2] - median[2]) * (median[2] - low[2]) / pow(high[2] - low[2], 2)){
			// True == splitting along x generates a more balanced partition
				first = new BinarySplitNode(low[0], low[1], low[2], high[0], median[1], high[2], level + 1);
				second = new BinarySplitNode(low[0], median[1], low[2], high[0], high[1], high[2], level + 1);
		
			} else {
				first = new BinarySplitNode(low[0], low[1], low[2], high[0], high[1], median[2], level + 1);
				second = new BinarySplitNode(low[0], low[1], median[2], high[0], high[1], high[2], level + 1);
			}

			#endif
			isLeaf = false;
			/* Split the node */

			for(vector<SpatialObject*>::iterator it = objectList.begin(); it != objectList.end(); it++ ) {
				/* Overflow cases -> reached max level of recursion */
				if (first->intersects(*it)) {
					//first->addObject(*it);
					first->addObjectIgnore(*it);	
					/* if (!first->addObject(*it)) {
					   canBeSplit = false;
					   isLeaf = true;
					   return false;
					   }*/
				}
				if (second->intersects(*it)) {
					//second->addObject(*it);
					second->addObjectIgnore(*it);
					/*if (!second->addObject(*it)) {
					  canBeSplit = false;
					  isLeaf = true;
					  return false;
					  }*/
				}

			}

			#ifdef DEBUG
			cerr << "Sizes: " << first->size << " " << second->size << endl;
			#endif
			
			//if (!first->canBeSplit && !second->canBeSplit) {
			if (first->size + second->size > 2 * (size - 1)) {
				isLeaf = true;
				canBeSplit = false;
				#ifdef DEBUG
				cerr << "Fail in finding good split point" << endl;
				#endif
				delete first;
				delete second;
			} else {
				#ifdef DEBUG
				cerr << "Succeed in splitting";
				#endif
				leafNodeList.push_back(first);
				leafNodeList.push_back(second);
				objectList.clear();
			}	  
		} else if (size > 1.5 * bucket_size) {
			canBeSplit = true;
		}

		/*
		   if (!isLeaf) {
		   objectList.clear();
		   }*/

	} else {

		if (first->intersects(object)) {
			first->addObject(object);
		}
		if (second->intersects(object)) {
			second->addObject(object);
		}
	}
	return true;
}


/* Check if the node MBB intersects with the object MBB */
bool BinarySplitNode::intersects(SpatialObject *object) {
	return !(object->low[0] > high[0] || object->high[0] < low[0]
		|| object->low[1] > high[1] || object->high[1] < low[1]
		|| object->low[2] > high[2] || object->high[2] < low[2]);
}
