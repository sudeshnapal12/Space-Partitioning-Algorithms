#include "str_3d.hpp"

/* Sort-tile recursive (STR) partitioning method */
using namespace std;
using namespace SpatialIndex;


class MyQueryStrategy : public SpatialIndex::IQueryStrategy
{
    private:
        queue<id_type> ids;
        struct partition_op *partop_ptr;
        id_type bid;
    public:
        MyQueryStrategy(struct partition_op *ptr) {
            partop_ptr = ptr;
            bid = 0;
        }

        void getNextEntry(const IEntry& entry, id_type& nextEntry, bool& hasNext)
        {
            const INode* n = dynamic_cast<const INode*>(&entry);

            // traverse only index nodes at levels 0 and higher.
            if (n != NULL) {
                if (n->getLevel() > 0)
                {
                    for (uint32_t cChild = 0; cChild < n->getChildrenCount(); cChild++)
                    {
                        ids.push(n->getChildIdentifier(cChild));
                    }
                }
                else if (n->getLevel() == 0)
                {
                    IShape* ps;
                    entry.getShape(&ps);
                    Region* pr = dynamic_cast<Region*>(ps);
                    //cout << *pr << "\n";

                    #ifdef DEBUGAREA
                        list_rec.push_back(new Cuboid(pr->m_pLow[0], pr->m_pLow[1], pr->m_pLow[2],
                            pr->m_pHigh[0], pr->m_pHigh[1], pr->m_pHigh[2]));
                    #endif

                    //cout << n->getIdentifier();
                    cout << partop_ptr->prefix_tile_id << bid<< TAB
                        << pr->m_pLow[0] << TAB << pr->m_pLow[1] << TAB << pr->m_pLow[2] << TAB
                        << pr->m_pHigh[0] << TAB << pr->m_pHigh[1] << TAB << pr->m_pHigh[2] << TAB
                        << endl;
                    bid++;
                    delete ps;
                }
            }

            if (!ids.empty()) {
                nextEntry = ids.front();
                ids.pop();
                hasNext = true;
            }
            else {
                hasNext = false;
            }
        }
};

// Customized reader for reading from a vector
class VecStreamReader : public IDataStream
{
    public:
        VecStreamReader(vector<RTree::Data*> * coll) : m_pNext(0)
        {
            if (coll->size()==0)
                throw Tools::IllegalArgumentException("Input vector is empty");

            sobjects = coll;
            index = 0 ;
            readNextEntry();
        }

        virtual ~VecStreamReader()
        {
            if (m_pNext != 0) delete m_pNext;
        }

        virtual IData* getNext()
        {
            if (m_pNext == 0) return 0;
            
            RTree::Data* ret = m_pNext;
            m_pNext = 0;
            readNextEntry();
            return ret;
        }

        virtual bool hasNext()
        {
            return (m_pNext != 0);
        }

        virtual uint32_t size()
        {
            throw Tools::NotSupportedException("Operation not supported.");
        }

        virtual void rewind()
        {
            if (m_pNext != 0)
            {
                delete m_pNext;
                m_pNext = 0;
            }
            index = 0;
            readNextEntry();
        }

        void readNextEntry()
        {
            if (index < sobjects->size())
            {
                m_pNext = (*sobjects)[index];
                index++;
            }
        }

        vector<RTree::Data*> * sobjects;
        vector<RTree::Data*>::size_type index ;
        RTree::Data* m_pNext;
};


void process_input(struct partition_op &partop) {
	// build in memory Tree
    VecStreamReader vecstream(&vec);
    IStorageManager* memoryFile = StorageManager::createNewMemoryStorageManager();
    id_type indexIdentifier = 0;

    ISpatialIndex* tree = RTree::createAndBulkLoadNewRTree(RTree::BLM_STR, 
        vecstream, *memoryFile, fillFactor, indexCapacity, partop.bucket_size, 
        3, SpatialIndex::RTree::RV_RSTAR, indexIdentifier);

    cout << "TREE\n" << *tree;

    bool ret = tree->isIndexValid();
    if (ret == false) std::cerr << "ERROR: Structure is invalid!" << std::endl;
    else std::cerr << "The stucture seems O.K." << std::endl;

    // Traverse through each leaf node which is a partition
    MyQueryStrategy qs(&partop);
    tree->queryStrategy(qs);

    #ifdef DEBUGAREA
        Cuboid *tmp;
        double span[3];
        double volume_total;

        for (int k = 0; k < 3; k++) {
            span[k] = partop.high[k] - partop.low[k];
        }

		// Normalize the data
        for (vector<Cuboid*>::iterator it = list_cub.begin() ; it != list_cub.end(); ++it) {
            tmp = *it;
            tmp->low[0] = (tmp->low[0] - partop.low[0]) / span[0];
            tmp->low[1] = (tmp->low[1] - partop.low[1]) / span[1];
            tmp->low[2] = (tmp->low[2] - partop.low[2]) / span[2];
            tmp->high[0] = (tmp->high[0] - partop.low[0]) / span[0];
            tmp->high[1] = (tmp->high[1] - partop.low[1]) / span[1];
            tmp->high[2] = (tmp->high[2] - partop.low[2]) / span[2];
            volume_total += (tmp->high[0] - tmp->low[0]) * (tmp->high[1] - tmp->low[1]) * (tmp->high[2] - tmp->low[2]);
        }

        cerr << "Volume total: " << volume_total << endl;
        if (list_cub.size() > 0)
        {
            cerr << "Volume covered: " << findvolume(list_cub) << endl;
        }

        for (vector<Cuboid*>::iterator it = list_cub.begin() ; it != list_cub.end(); ++it) {
            delete *it;
        }
        list_cub.clear();
    #endif 
	/* This will fail the code the Data is already removed by VecStreamReader
	for (vector<RTree::Data*>::iterator it = vec.begin() ; it != vec.end(); ++it) {
		delete *it;
	}*/
    vec.clear();
    delete tree;
    delete memoryFile;
}

bool read_input(struct partition_op &partop, char* argv[]) {
    cout << "read_input \n";
    string input_line;
    string prevtileid = "";
    string tile_id;
    vector<string> fields;
    double low[3];
    double high[3];

    RTree::Data *obj;
    id_type id = 0;

	partop.object_count = 0;
	ifstream file(argv[1]);
    while (file && getline(file, input_line)) {
		try {
			istringstream ss(input_line);
			ss >> tile_id >> low[0] >> low[1] >> low[2] >> high[0] >> high[1] >> high[2];
			
			cout << "input_line" << input_line << "\n";
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
            Region r(low, high, 3);
            obj = new RTree::Data(0, 0 , r, id);
			id++;
			vec.push_back(obj);

			fields.clear();
            partop.object_count++;

		} catch (...) {
            return false;
		}
	}
	if (partop.object_count > 0) {
		cout << "sud2 \n";
		// Process last tile
		if (partop.region_mbbs.size() == 1) {
			cout << "sud3 \n";
			update_partop_info(partop, prevtileid, prefix_tile_id);
		} else {
			cout << "sud4 \n";
			// First level of partitioning
			update_partop_info(partop, prevtileid, prevtileid + prefix_tile_id);
		}
		process_input(partop);
	}
	//total_count += partop.object_count;
	cleanup(partop);
    return true;
}

int main(int argc, char* argv[]) {
    cout.precision(15);
    struct partition_op partop;
    if (!extract_params_partitioning(argc, argv, partop)) {
        #ifdef DEBUG
            cerr << "Fail to extract parameters" << endl;
        #endif
        return -1;
    }

    if (!read_input(partop, argv)) {
		cerr << "Error reading input in." << endl;
		return -1;
    }
    cerr.flush();
    cout.flush();
}