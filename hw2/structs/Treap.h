#pragma once
#include "IDataStruct.h"
#include "../../utils/Random.hpp"
#include <iostream>
#include <stack>
#include <utility>
#include <math.h>

class Treap : public IDataStruct {
private:
	NodePtr root_ = nullptr;
	Random rng_;

	void clear(NodePtr node) {
		if (!node) return;
		for (auto c : node->children) {
			clear(c);
		}
		delete node;
	}

	NodePtr rotateRight(NodePtr n) {
		NodePtr l = n->children[0];
		n->children[0] = l->children[1];
		l->children[1] = n;
		return l;
	}
	NodePtr rotateLeft(NodePtr n) {
		NodePtr r = n->children[1];
		n->children[1] = r->children[0];
		r->children[0] = n;
		return r;
	}

public:
	Treap(int seed) : rng_(seed) {
		rng_.setDoubleRange(0, 1);
	}
	~Treap() override {
		clear(root_);
		root_ = nullptr;
	}

	NodePtr Create(int id, int score) override {
		return Insert(id, score);
	}
	NodePtr Create(int id, int score, double priority) {
		return Insert(id, score, priority);
	}

	NodePtr Insert(int id, int score) override {
		double pri = rng_.nextDouble();
		return Insert(id, score, pri);
	}
	NodePtr Insert(int id, int score, double priority) {
		root_ = Insert(root_, id, score, priority);
		return root_;
	}
	NodePtr Insert(NodePtr node, int id, int score, double priority) {
		if (!node) {
			node = new Node(id);
			node->children = {nullptr, nullptr};
			node->priority = priority;
			node->scores.push_back(score);
			return node;
		}

		if (id < node->id) {
			node->children[0] = Insert(node->children[0], id, score, priority);
			if (node->children[0]->priority > node->priority) node = rotateRight(node);
		}
		else if (id > node->id) {
			node->children[1] = Insert(node->children[1], id, score, priority);
			if (node->children[1]->priority > node->priority) node = rotateLeft(node);
		}
		else {
			node->scores.push_back(score);
		}

		return node;
	}

	void Print() override {
		if (!root_) {
			std::cout << "empty" << std::endl;
			return;
		}
		std::stack<std::pair<NodePtr, int>> stk; // nodeptr, depth
		stk.push({root_, 0});

		while (!stk.empty()) {
			auto node = stk.top().first;
			int depth = stk.top().second;
			stk.pop();

			for (int i=0; i<depth; ++i) {
				std::cout << (i+1 == depth ? "|-" : "| ");
			}

			if (!node) {
				std::cout << "leaf" << std::endl;
				continue;
			}
			else std::cout << node->id << std::endl;
			
			for (int i=1; i>=0; --i) {
				stk.push({node->children[i], depth + 1});
			}
		}
	}

	int Height() override {
		if (!root_) return -1;

		int ans = 0;
		std::stack<std::pair<NodePtr, int>> stk; // nodeptr, depth
		stk.push({root_, 0});

		while (!stk.empty()) {
			auto node = stk.top().first;
			int depth = stk.top().second;
			stk.pop();
			
			for (const auto& c : node->children) {
				if (!c) continue;
				ans = std::max(ans, depth + 1);
				stk.push({c, depth + 1});
			}
		}

		return ans;
	}

	double SearchAVG(int id) override {
		NodePtr cur = root_;

		while(cur != nullptr) {
			if (id == cur->id) {
				double total = 0;
				int count = 0;
				for (const auto& s : cur->scores) {
					total += s;
					++count;
				}
				return total / count;
			}
			else if (id < cur->id) cur = cur->children[0];
			else cur = cur->children[1];
		}

		return -1;
	}


};