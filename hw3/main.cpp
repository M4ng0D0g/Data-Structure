#include "structs.hpp"
#include "../utils/Random.hpp"
#include <iostream>
#include <unordered_set>
#include <chrono>

using namespace std;
using namespace chrono;

// const size_t ID_SIZE = (1 << 20);
const pair<int, int> ID_RANGE{1, 1 << 20};
const pair<int, int> SCORE_RANGE{0, 100};

const int RANDOM_SEED = 42;

/****************************************************************************************************/

template <typename DataStruct, typename... Args>
void experiment(int dataSize, int seed, Args&&... args) {
	Random ID_RNG(seed);
	Random SCORE_RNG(seed + 1);
	Random SEARCH_RNG(seed + 2);

	ID_RNG.setIntRange(ID_RANGE.first, ID_RANGE.second); 
	SCORE_RNG.setIntRange(SCORE_RANGE.first, SCORE_RANGE.second); 

	milliseconds insert_dur{0}, search_dur{0};

	for (int t = 0; t < 10; ++t) {
		DataStruct ds(forward<Args>(args)...);
		unordered_set<int> idSet;
		vector<int> insertedIds;
		
		// --------------------------------------------------------------------------------
		// Insert

		auto insert_start = steady_clock::now();

		for (int i = 0; i < dataSize; ++i) {
			auto id = ID_RNG.nextInt();
			ds.Insert(id, SCORE_RNG.nextInt());
			if (!idSet.count(id)) insertedIds.push_back(id);
		}

		auto insert_end = steady_clock::now();
		insert_dur += duration_cast<milliseconds>(insert_end - insert_start);

		// --------------------------------------------------------------------------------
		// search

		SEARCH_RNG.setIntRange(0, insertedIds.size()-1);

		auto search_start = steady_clock::now();

		for (int i = 0; i < 100000; ++i) {
			auto query = insertedIds[SEARCH_RNG.nextInt()];
			ds.SearchAVG(query);
		}

		auto search_end = steady_clock::now();
		search_dur += duration_cast<milliseconds>(search_end - search_start);

	}

	cout << "Insert Time: " << insert_dur.count() / 10
	<< " ms Search Time: " << search_dur.count() / 10 << endl;
}

int main() {
	ios::sync_with_stdio(0); cin.tie(0);

	cout << "[Experiment]" << endl;

	// BST
	cout << "---------- BST ----------" << endl;
	for (int i = 10; i <= 20; ++i) {
		cout << "Data Size: 2^" << i << " ";
		experiment<BST>(1 << i, RANDOM_SEED);
	}
	cout << endl;

	// HT
	cout << "---------- HT ----------" << endl;
	for (int i = 10; i <= 20; ++i) {
		cout << "Data Size: 2^" << i << " ";
		experiment<HT>(1 << i, RANDOM_SEED);
	}
	cout << endl;

}