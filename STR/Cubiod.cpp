#include <iostream>
#include <vector>

using namespace std;


class Cubiod {
	public:
		double low[3];
		double high[3];
		Cuboid(double x1, double y1, double z1, double x2, double y2, double z2) {
			low[0] = x1;
			low[1] = y1;
			low[2] = z1;
			high[0] = x2;
			high[1] = y2; 
			high[2] = z2;
		};
		void print(void) {
			cout << "Cuboid: " << low[0] << " " << low[1] << " " << low[2] << " " 
			<< high[0] << " " << high[1] << " " << high[2] << " " <<endl;
		};
};

// return the iterator of rec in list
vector<Cuboid *>::iterator bin_search(vector<Cuboid *> &list, int begin, int end, Cuboid *rec) {
	int mid = (begin + end)/2;
	if (list[mid]->low[1] == rec->low[1]) {
		if (list[mid]->high[1] == rec->high[1])
			return list.begin() + mid;
		else if (list[mid]->high[1] < rec->high[1]) {
			if (mid == end)
				return list.begin() + mid+1;
			return bin_search(list,mid+1,mid,rec);
		}
		else {
			if (mid == begin)
				return list.begin()+mid;
			return bin_search(list,begin,mid-1,rec);
		}
	}
	else if (list[mid]->low[1] < rec->low[1]) {
		if (mid == end) {
			return list.begin() + mid+1;
		}
		return bin_search(list, mid+1, end, rec);
	}
	else {
		if (mid == begin) {
			return list.begin() + mid;
		}
		return bin_search(list, begin, mid-1, rec);
	}
}

// add cub to cubs
void add_cub(Cuboid *cub, vector<Cuboid *> &cubs) {
	if (cubs.size() == 0) {
		cubs.push_back(cub);
	}
	else {
		vector<Cuboid *>::iterator it = bin_search(cubs, 0, cubs.size()-1, cub);
		cubs.insert(it, cub);
	}
}

// remove rec from rets
void remove_cub(Cuboid *cub, vector<Cuboid *> &cubs) {
	vector<Cuboid *>::iterator it = bin_search(cubs, 0, cubs.size()-1, cub);
	cubs.erase(it);
}

// calculate the total vertical length covered by cuboids in the active set
double vert_dist(vector<Cuboid *> as) {
	int n = as.size();
	double totallength = 0;
	double start, end;

	int i = 0;
	while (i < n) {
		start = as[i]->low[1];
		end = as[i]->high[1];
		while (i < n && as[i]->low[1] <= end) {
			if (as[i]->high[1] > end) {
				end = as[i]->high[1];
			}
			i++;
		}
		totallength += end - start;
	}
	return totallength;
}

// calculate the total depth length covered by cuboids in the active set
double depth_dist(vector<Cuboid *> as) {
	int n = as.size();
	double totallength = 0;
	double start, end;

	int i = 0;
	while (i < n) {
		start = as[i]->low[2];
		end = as[i]->high[2];
		while (i < n && as[i]->low[2] <= end) {
			if (as[i]->high[2] > end) {
				end = as[i]->high[2];
			}
			i++;
		}
		totallength += end - start;
	}
	return totallength;
}

bool mycomp1(Cuboid* a, Cuboid* b) {
	return (a->low[0] < b->low[0]);
}

bool mycomp2(Cuboid* a, Cuboid* b) {
	return (a->high[0] < b->high[0]);
}

double findvolume(vector<Cuboid *> &cubs) {
	vector<Rectangle *> start = cubs;
	vector<Rectangle *> end = cubs;
	sort(start.begin(), start.end(), mycomp1);
	sort(end.begin(), end.end(), mycomp2);

	// active set
	vector<Cuboid *> as;

	int n = cubs.size();

	double totalvolume = 0;
	double current = start[0]->low[0];
	double next;
	int i = 0, j = 0;
	// big loop
	while (j < n) {
	//	cout << "loop---------------"<<endl;
		// add all cubs that start at current
		while (i < n && start[i]->low[0] == current) {
	//		cout << "add" <<endl;
			// add start[i] to AS
			add_cub(start[i], as);
	//		cout << "after" <<endl;
			i++;
		}
		// remove all cubs that end at current
		while (j < n && end[j]->high[0] == current) {
	//		cout << "remove" <<endl;
			// remove end[j] from AS
			remove_cub(end[j], as);
	//		cout << "after" <<endl;
			j++;
		}

		// find next event x
		if (i < n && j < n) {
			if (start[i]->low[0] <= end[j]->high[0]) {
				next = start[i]->low[0];
			}
			else {
				next = end[j]->high[0];
			}
		}
		else if (j < n) {
			next = end[j]->high[0];
		}

		// distance to next event
		double horiz = next - current;

		// figure out vertical dist
		double vert = vert_dist(as);

		// figure out depth dist
		double depth = depth_dist(as);

		totalvolume += vert * horiz * depth;

		current = next;
	}
	return totalvolume;
}
