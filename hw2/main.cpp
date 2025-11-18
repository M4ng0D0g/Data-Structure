#include "structs/AVL.h"
#include "structs/BST.h"
#include "structs/SkipList.h"
#include "structs/Treap.h"
#include <bits/stdc++.h>
#include "../utils/Random.hpp"

using namespace std;
using namespace chrono;

const size_t ID_SIZE = (1 << 20);
const pair<int, int> ID_RANGE{1, 1 << 20};
const pair<int, int> SCORE_RANGE{0, 100};

const int RANDOM_SEED = 135;

/****************************************************************************************************/

void test() {
	vector<int> Ids;
	int RdIdx;
	Random RNG;

	// BST
	cout << "---------- BST ----------" << endl;
	BST bst;
	bst.Insert(3, 100); bst.Insert(2, 60); bst.Insert(1, 70); bst.Insert(5, 40); bst.Insert(4, 70);
	bst.Print();
	cout << "Height: " << bst.Height() << endl;
	
	Ids = {3, 2, 1, 5, 4};
	RNG.setIntRange(0, Ids.size()-1);
	RdIdx = RNG.nextInt();
	cout << "Random search(Id = " << Ids[RdIdx] << "): " << bst.SearchAVG(Ids[RdIdx]) << endl;
	cout << endl;

	// AVL
	cout << "---------- AVL ----------" << endl;
	AVL avl;
	avl.Insert(1, 100); avl.Insert(2, 60); avl.Insert(3, 70); avl.Insert(4, 40); avl.Insert(5, 70);
	avl.Print();
	cout << "Height: " << avl.Height() << endl;
	
	Ids = {1, 2, 3, 4, 5};
	RNG.setIntRange(0, Ids.size()-1);
	RdIdx = RNG.nextInt();
	cout << "Random search(Id = " << Ids[RdIdx] << "): " << avl.SearchAVG(Ids[RdIdx]) << endl;
	cout << endl;

	// Treap
	cout << "---------- Treap ----------" << endl;
	Treap treap(RANDOM_SEED);
	treap.Insert(3, 100, 0.9); treap.Insert(2, 60, 0.5); treap.Insert(1, 70, 0.3);
	treap.Insert(5, 60, 0.2); treap.Insert(4, 80, 0.1);
	treap.Print();
	cout << "Height: " << treap.Height() << endl;
	
	Ids = {3, 2, 1, 5, 4};
	RNG.setIntRange(0, Ids.size()-1);
	RdIdx = RNG.nextInt();
	cout << "Random search(Id = " << Ids[RdIdx] << "): " << treap.SearchAVG(Ids[RdIdx]) << endl;
	cout << endl;

	// Skip List
	cout << "---------- Skip List ----------" << endl;
	SkipList skipList(RANDOM_SEED);
	skipList.Insert(1, 100, 3); skipList.Insert(2, 60, 1); skipList.Insert(3, 70, 2); skipList.Insert(4, 80, 4);
	skipList.Insert(5, 60, 1); skipList.Insert(6, 80, 1); skipList.Insert(7, 90, 3);
	skipList.Print();
	cout << "Height: " << skipList.Height() << endl;
	
	Ids = {1, 2, 3, 4, 5, 6, 7};
	RNG.setIntRange(0, Ids.size()-1);
	RdIdx = RNG.nextInt();
	cout << "Random search(Id = " << Ids[RdIdx] << "): " << skipList.SearchAVG(Ids[RdIdx]) << endl;
	cout << endl;

}

template <typename DataStruct, typename... Args>
void experiment(int dataSize, int seed, Args&&... args) {
	Random ID_RNG(seed);
	Random SCORE_RNG(seed + 1);
	Random SEARCH_RNG(seed + 2);

	ID_RNG.setIntRange(ID_RANGE.first, ID_RANGE.second); 
	SCORE_RNG.setIntRange(SCORE_RANGE.first, SCORE_RANGE.second); 

	milliseconds insert_dur{0}, search_dur{0};
	double height_avg;

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

		// --------------------------------------------------------------------------------
		// height

		height_avg += ds.Height();
	}

	cout << "Insert Time: " << insert_dur.count() / 10
	<< " ms Search Time: " << search_dur.count() / 10
	<< "ms AVG Height: " << height_avg / 10 << endl;
}

int main() {
	ios::sync_with_stdio(0); cin.tie(0);

	cout << "[Test]" << endl;
	test();
	cout << endl;

	cout << "[Experiment]" << endl;
	// BST
	cout << "---------- BST ----------" << endl;
	for (int i = 10; i <= 20; ++i) {
		cout << "Data Size: 2^" << i << " ";
		experiment<BST>(1 << i, RANDOM_SEED);
	}
	cout << endl;

	// AVL
	cout << "---------- AVL ----------" << endl;
	cout << "limit = 1" << endl;
	for (int i = 10; i <= 20; ++i) {
		cout << "Data Size: 2^" << i << " ";
		experiment<AVL>(1 << i, RANDOM_SEED, 1);
	}
	cout << endl;

	cout << "limit = 3" << endl;
	for (int i = 10; i <= 20; ++i) {
		cout << "Data Size: 2^" << i << " ";
		experiment<AVL>(1 << i, RANDOM_SEED, 3);
	}
	cout << endl;
	
	// Treap
	cout << "---------- Treap ----------" << endl;
	for (int i = 10; i <= 20; ++i) {
		cout << "Data Size: 2^" << i << " ";
		experiment<Treap>(1 << i, RANDOM_SEED, RANDOM_SEED);
	}
	cout << endl;
	
	// Skip List
	cout << "---------- Skip List ----------" << endl;
	cout << "pr[H] = 0.25" << endl;
	for (int i = 10; i <= 20; ++i) {
		cout << "Data Size: 2^" << i << " ";
		experiment<SkipList>(1 << i, RANDOM_SEED, RANDOM_SEED, 0.25);
	}
	cout << endl;
	
	cout << "pr[H] = 0.5" << endl;
	for (int i = 10; i <= 20; ++i) {
		cout << "Data Size: 2^" << i << " ";
		experiment<SkipList>(1 << i, RANDOM_SEED, RANDOM_SEED, 0.5);
	}
	cout << endl;
	
	cout << "pr[H] = 0.75" << endl;
	for (int i = 10; i <= 20; ++i) {
		cout << "Data Size: 2^" << i << " ";
		experiment<SkipList>(1 << i, RANDOM_SEED, RANDOM_SEED, 0.75);
	}
	cout << endl;
}