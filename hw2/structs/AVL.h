#pragma once
#include "IDataStruct.h"
#include <iostream>
#include <stack>
#include <utility>
#include <math.h>

class AVL : public IDataStruct {
private:
	NodePtr root_ = nullptr;
	int limit_;

	void clear(NodePtr node) {
		if (!node) return;
		for (auto c : node->children) {
			clear(c);
		}
		delete node;
	}

	int height(NodePtr n) {
		return n ? n->height : -1;
	}
	void updateHeight(NodePtr n) {
		if (n) {
			n->height = 1 + std::max(height(n->children[0]), height(n->children[1]));
		}

	}
	int balanceFactor(NodePtr n) {
		return n ? height(n->children[0]) - height(n->children[1]) : 0;
	}
	NodePtr rotateRight(NodePtr n) {
		NodePtr l = n->children[0];
		NodePtr	lr = l->children[1];

		l->children[1] = n;
		n->children[0] = lr;

		updateHeight(n);
		updateHeight(l);
		return l;
	}
	NodePtr rotateLeft(NodePtr n) {
		NodePtr r = n->children[1];
		NodePtr	rl = r->children[0];

		r->children[0] = n;
		n->children[1] = rl;

		updateHeight(n);
		updateHeight(r);
		return r;
	}
	NodePtr rebalance(NodePtr n) {
		int bf = balanceFactor(n);
		if (bf > limit_) {
			if (balanceFactor(n->children[0]) < 0) {
				n->children[0] = rotateLeft(n->children[0]);
			}
			return rotateRight(n);
		}
		else if (bf < -limit_) {
			if (balanceFactor(n->children[1]) > 0) {
				n->children[1] = rotateRight(n->children[1]);
			}
			return rotateLeft(n);
		}
		return n;
	}

public:
	AVL(int limit = 1) : limit_(limit) {}
	~AVL() override {
		clear(root_);
		root_ = nullptr;
	}

	NodePtr Create(int id, int score) override {
		root_ = new Node(id);
		root_->children = {nullptr, nullptr};
		root_->scores.push_back(score);
		return root_;
	}

	NodePtr Insert(int id, int score) override {
		root_ = Insert(root_, id, score);
		return root_;
	}
	NodePtr Insert(NodePtr node, int id, int score) {
		if (!node) {
			node = new Node(id);
			node->children = {nullptr, nullptr};
			node->scores.push_back(score);
			node->height = 1;
			return node;
		}

		if (id < node->id) node->children[0] = Insert(node->children[0], id, score);
		else if (id > node->id) node->children[1] = Insert(node->children[1], id, score);
		else {
			node->scores.push_back(score);
			return node;
		}

		updateHeight(node);
		return rebalance(node);
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
		return height(root_);
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