#include "./utils/dynamic_array.hpp"
#include <bits/stdc++.h>

using namespace std;

size_t constexpr IDSIZE = (1 << 20);

struct Node1 {
	int id;
	vector<int> score;
};

struct Node2 {
	int id;
	vector<int> score;
	int next;
};

struct Node3 {
	int id;
	vector<int> score;
	Node3* next;
};

class Sol {
public:
	virtual void insert(int id, int score) = 0;
	virtual vector<int> search(int id) = 0;
	virtual void total(size_t dataSize) = 0;

	virtual std::shared_ptr<Sol> clone() const = 0;
};

/****************************************************************************************************/

class Sol1 : public Sol {
private:
	vector<Node1*> arr_;

public:
	Sol1() : arr_() {
		arr_.reserve(1);
	}
	~Sol1() {
		for (auto node : arr_) {
			delete node;
		}
	}
	std::shared_ptr<Sol> clone() const override {
		return std::make_shared<Sol1>(*this);
	}

	void insert(int id, int score) override {
		auto pos = lower_bound(arr_.begin(), arr_.end(), id,
			[](const Node1* a, int value) {
				return a->id < value;
			});
		
		if (pos != arr_.end() && (*pos)->id == id) (*pos)->score.push_back(score);
		else {
			if (arr_.size() >= arr_.capacity()) {
				auto idx = pos - arr_.begin();
				arr_.reserve(arr_.capacity() * 10);
				pos = arr_.begin() + idx;
			}
			arr_.insert(pos, new Node1{id, {score}});
		}
	}

	vector<int> search(int id) override {
		auto pos = lower_bound(arr_.begin(), arr_.end(), id,
			[](const Node1* a, int value) {
				return a->id < value;
			});
		if (pos != arr_.end() && (*pos)->id == id) return (*pos)->score;
		return {-1};
	}

	void total(size_t dataSize) override {
		for (auto& a : arr_) {
			volatile long long total{0};
			if (a->score.size() != 0) for (const auto& s : a->score) total += s;
		}
	}
};

class Sol2 : public Sol {
private:
	vector<Node2*> arr_;
	int head_;
	const int SIZE_SQRT_;
	int size_;

public:
	Sol2(size_t dataSize) : arr_(IDSIZE + 1, nullptr), head_(-1), SIZE_SQRT_(sqrt(dataSize)), size_(0) {}
	~Sol2() {
		for (auto node : arr_) delete node;
	}
	std::shared_ptr<Sol> clone() const override {
		return std::make_shared<Sol2>(*this);
	}

	void insert(int id, int score) override {
		if (arr_[id]) {
			arr_[id]->score.push_back(score);
			return;
		}

		auto node = new Node2{id, {score}, -1};
		arr_[id] = node;
		++size_;

		/*
		id之前的機率為 xBar = id/idSize, 前方期望個數 prevN = size * xBar
		前方空間 prevS = id, 平均空間切割 prevDS = prevS / prevN
		當 prevDS > prevN 適合用 list搜, 否則用 arr搜
		size_t prevN = round(static_cast<double>(id) / IDSIZE * size_);
		size_t prevDS = round(id / max((size_t)1, prevN));
		double ratio = static_cast<double>(id) / IDSIZE;
		double expectedPrev = size_ * ratio;
		// double expectedNext = size_ - expectedPrev;
		double expectedSpace = static_cast<double>(id) / expectedPrev;
		bool useForward = expectedPrev * 4 < expectedSpace;

		if (useForward) {
			int prev = -1;
			int cur = head_;
			while (cur != -1 && cur < id) {
				prev = cur;
				cur = arr_[cur]->next;
			}
			node->next = cur;
			if (prev == -1) head_ = id;
			else arr_[prev]->next = id;
		}
		*/
		
		// else {
		int back;
		for (back = id-1; back > 0; --back) {
			if (arr_[back]) {
				arr_[id]->next = arr_[back]->next;
				arr_[back]->next = id;
				break;
			}
		}
		if (back <= 0) return;

		head_ = id;
		for (int front = id+1; front < IDSIZE + 1; ++front) {
			if (arr_[front]) {
				arr_[id]->next = front;
				break;
			}
		}
		// }
	}

	vector<int> search(int id) override {
		if (!arr_[id]) return {-1};
		return arr_[id]->score;
	}

	void total(size_t dataSize) override {
		int temp = head_;
		while (temp != -1) {
			// cout << temp << endl;
			volatile long long total{0};
			if (arr_[temp]) for (const auto& s : arr_[temp]->score) total += s;
			temp = arr_[temp]->next;
		}
	}
};

class Sol3 : public Sol {
private:
	Node3* head_;

public:
	Sol3() : head_(nullptr) {}
	~Sol3() {
		while (head_) {
			Node3* temp = head_;
			head_ = head_->next;
			delete temp;
		}
	}
	std::shared_ptr<Sol> clone() const override {
		return std::make_shared<Sol3>(*this);
	}

	void insert(int id, int score) override {
		// find
		auto pos = head_;
		while (pos) {
			if (pos->id == id) {
				pos->score.push_back(score);
				return;
			}
			else pos = pos->next;
		}
		// create
		auto node = new Node3{id, {score}, nullptr};

		if (!head_) {
			head_ = node;
			return;
		}

		Node3** temp = &head_;
		while (*temp != nullptr && node > *temp) {
			temp = &(*temp)->next;
		}
		node->next = *temp;
		*temp = node;
	}

	vector<int> search(int id) override {
		auto temp = head_;
		while (temp) {
			if (temp->id == id) return temp->score;
			temp = temp->next;
		}
		return {-1};
	}

	void total(size_t dataSize) override {
		auto cur = head_;
		while (cur) {
			volatile long long total{0};
			if (cur->score[0] != -1) for (const auto& s : cur->score) total += s;
			cur = cur->next;
		}
	}
};

/****************************************************************************************************/

void test(shared_ptr<Sol> sol_t, size_t dataSize, int seed) {
	mt19937 rng(seed);
	uniform_int_distribution<size_t> idDist(1, IDSIZE);
	uniform_int_distribution<size_t> scoreDist(0, 100);
	
	chrono::milliseconds insert_dur{0}, search_dur{0}, total_dur{0};

	for (int t = 0; t < 10; ++t) {
		auto sol = sol_t->clone();
		
		// insert
		auto insert_start = chrono::high_resolution_clock::now();

		for (int i = 0; i < dataSize; ++i) {
			sol->insert(idDist(rng), scoreDist(rng));
		}

		auto insert_end = chrono::high_resolution_clock::now();
		insert_dur += chrono::duration_cast<chrono::milliseconds>(insert_end - insert_start);

		// search
		auto search_start = chrono::high_resolution_clock::now();

		for (int i = 0; i < 1e5; ++i) {
			sol->search(idDist(rng));
		}

		auto search_end = chrono::high_resolution_clock::now();
		search_dur += chrono::duration_cast<chrono::milliseconds>(search_end - search_start);

		// total
		auto total_start = chrono::high_resolution_clock::now();

		sol->total(dataSize);

		auto total_end = chrono::high_resolution_clock::now();
		total_dur += chrono::duration_cast<chrono::milliseconds>(total_end - total_start);
	}

	cout << "Insert: " << insert_dur.count() / 10
	<< " ms Search: " << search_dur.count() / 10
	<< "ms Total: " << total_dur.count() / 10 << "ms" << endl;
}

void extra(shared_ptr<Sol> sol_t, int seed) {
	const int DataSize = 1 << 20;

	mt19937 rng(seed);
	
	vector<int> idList{};
	for (int i = 1; i < IDSIZE + 1; ++i) idList.push_back(i);
	shuffle(idList.begin(), idList.end(), rng);

	for (int d = 1; d < 21; d += 1) {
		cout << "Density: " << d << " ";

		uniform_int_distribution<size_t> idDist(1, (1 << d));
		uniform_int_distribution<size_t> scoreDist(0, 100);

		chrono::milliseconds insert_dur{0};

		for (int t = 0; t < 10; ++t) {
			auto sol = sol_t->clone();

			// insert
			auto insert_start = chrono::high_resolution_clock::now();

			for (int i = 0; i < DataSize; ++i) {
				sol->insert(idList[idDist(rng)+1], scoreDist(rng));
			}

			auto insert_end = chrono::high_resolution_clock::now();
			insert_dur += chrono::duration_cast<chrono::milliseconds>(insert_end - insert_start);

			// search
			// auto search_start = chrono::high_resolution_clock::now();

			// for (int i = 0; i < 1e5; ++i) {
			// 	sol->search(idDist(rng));
			// }

			// auto search_end = chrono::high_resolution_clock::now();
			// search_dur += chrono::duration_cast<chrono::milliseconds>(search_end - search_start);
		}
		cout << "Insert: " << insert_dur.count() / 10 << " ms" << endl;
	}
}

int main() {
	ios::sync_with_stdio(0); cin.tie(0);

	int seed = 114514;

	// cout << "= Solution 1 =" << endl;
	// for (int c = 11 ; c <= 25; ++c) {
	// 	size_t DATASIZE = 1 << c;
	// 	cout << "Data size: 2^" << c << " ";
	// 	test(make_shared<Sol1>(), DATASIZE, seed);
	// }
	// cout << "= Solution 1 =" << endl;
	// extra(make_shared<Sol1>(), seed);

	// cout << "= Solution 2 =" << endl;
	// for (int c = 11; c <= 25; ++c) {
	// 	size_t DATASIZE = 1 << c;
	// 	cout << "Data size: 2^" << c << " ";
	// 	test(make_shared<Sol2>(DATASIZE), DATASIZE, seed);
	// }
	// cout << "= Solution 2 =" << endl;
	// extra(make_shared<Sol2>(1 << 20), seed);

	cout << "= Solution 3 =" << endl;
	for (int c = 11; c <= 25; ++c) {
		size_t DATASIZE = 1 << c;
		cout << "Data size: 2^" << c << " ";
		test(make_shared<Sol3>(), DATASIZE, seed);
	}
	// cout << "= Solution 3 =" << endl;
	// extra(make_shared<Sol3>(), seed);

}
