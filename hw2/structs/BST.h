#pragma once
#include "IDataStruct.h"
#include <iostream>
#include <stack>
#include <utility>
#include <math.h>

class BST : public IDataStruct {
private:
	NodePtr root_ = nullptr;

	void clear(NodePtr node) {
		if (!node) return;
		for (auto c : node->children) {
			clear(c);
		}
		delete node;
	}

public:
	~BST() override {
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
		NodePtr *cur = &root_;

		while(*cur) {
			if (id == (*cur)->id) {
				(*cur)->scores.push_back(score);
				return root_;
			}
			else if (id < (*cur)->id) cur = &(*cur)->children[0];
			else cur = &(*cur)->children[1];
		}

		*cur = new Node(id);
		(*cur)->children = {nullptr, nullptr};
		(*cur)->scores.push_back(score);
		return root_;
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