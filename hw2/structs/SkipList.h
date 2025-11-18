#pragma once
#include "IDataStruct.h"
#include "../../utils/Random.hpp"
#include <limits>
#include <deque>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

class SkipList : public IDataStruct {
private:
	Random rng_;
	double prHead_;

	NodePtr head_, tail_;
	int height_ = 1;

	void clear(Node* node) {
		if (!node) return;

		std::stack<Node*> stk;
		std::unordered_set<Node*> visited;  // 記錄已訪問節點，避免重複 delete

		stk.push(node);

		while (!stk.empty()) {
			Node* n = stk.top();
			stk.pop();

			if (!n || visited.count(n)) continue;  // 已訪問過，跳過
			visited.insert(n);

			// 將子節點入棧
			for (auto c : n->children) {
				if (c) stk.push(c);
			}

			// 刪除自己
			delete n;
		}
	}

public:
    SkipList(int seed, double prHead = 0.5) : rng_(seed), prHead_(prHead) {
        rng_.setDoubleRange(0.0, 1.0);

        tail_ = new Node(INT32_MAX);
        tail_->children = {nullptr, nullptr};

        head_ = new Node(INT32_MIN);
        head_->children = {tail_, nullptr};
    }
    ~SkipList() override {
        clear(head_);
        head_ = nullptr;
        tail_ = nullptr;
    }

    NodePtr Create(int id, int score) override {
        return Insert(id, score);
    }

    NodePtr Create(int id, int score, int HCount) {
        return Insert(id, score, HCount);
    }

    NodePtr Insert(int id, int score) override {
        int level = 1;
        while (rng_.nextDouble() < prHead_) ++level;
        return Insert(id, score, level);
    }

    NodePtr Insert(int id, int score, int HCount) {
        std::deque<NodePtr> lastNodes;

        NodePtr cur = head_;
        while (cur) {
            while (!cur->children.empty() && cur->children[0] && cur->children[0]->id < id) {
                cur = cur->children[0];
            }

            if (!cur->children.empty() && cur->children[0] && cur->children[0]->id == id) {
                cur = cur->children[0];
                while (cur->children.size() > 1 && cur->children[1]) {
                    cur = cur->children[1];
                }
                cur->scores.push_back(score);
                return head_;
            } else {
                lastNodes.push_back(cur);
                if (cur->children.size() > 1)
                    cur = cur->children[1];
                else
                    break;
            }
        }

        NodePtr curHead = head_, curTail = tail_;
        for (; height_ < HCount; ++height_) {
            NodePtr newTail = new Node(INT32_MAX);
            newTail->children = {nullptr, curTail};

            NodePtr newHead = new Node(INT32_MIN);
            newHead->children = {newTail, curHead};

            lastNodes.push_front(newHead);
            curHead = newHead;
            curTail = newTail;
        }

        head_ = curHead;
        tail_ = curTail;

        NodePtr prevNode = nullptr;
        for (int i = 0; i < HCount; ++i) {
            NodePtr pos = lastNodes.back();
            lastNodes.pop_back();

            NodePtr newNode = new Node(id);
            newNode->children = {pos->children[0], prevNode};
            if (i == 0) newNode->scores.push_back(score);
            pos->children[0] = newNode;

            prevNode = newNode;
        }

        return head_;
    }

    double SearchAVG(int id) override {
        NodePtr cur = head_;
        while (cur) {
            while (!cur->children.empty() && cur->children[0] && cur->children[0]->id < id) {
                cur = cur->children[0];
            }

            if (!cur->children.empty() && cur->children[0] && cur->children[0]->id == id) {
                cur = cur->children[0];
                while (cur->children.size() > 1 && cur->children[1]) {
                    cur = cur->children[1];
                }
                if (cur->scores.empty()) return -1;
                double total = 0.0;
                for (auto s : cur->scores) total += s;
                return total / cur->scores.size();
            } else {
                if (cur->children.size() > 1)
                    cur = cur->children[1];
                else
                    break;
            }
        }
        return -1;
    }

	int Height() override {
		return height_;
	}

	void Print() override {
		int bufferSize = height_*2-1;
		std::vector<std::string> buffer(bufferSize);
		std::unordered_map<int, int> id_idx;

		std::vector<NodePtr> stk{head_};
		while(stk.back()->children[1]) stk.push_back(stk.back()->children[1]);
		
		auto node = stk.back(); stk.pop_back();
		while (node) {
			if (node->id == INT32_MIN) buffer[0].append("MIN-");
			else if (node->id == INT32_MAX) buffer[0].append("MAX");
			else {
				id_idx[node->id] = buffer[0].size();
				buffer[0].append(std::to_string(node->id) + "-");
			}
			node = node->children[0];
		}

		int size = buffer[0].size();
		for (int i=2; i<bufferSize; i+=2) {
			buffer[i].append("MIN-");
			buffer[i].append(size-7, '-');
			buffer[i].append("MAX");
		}
		for (int i=1; i<bufferSize; i+=2) {
			buffer[i].append(size, ' ');
			buffer[i][1] = '|';
			buffer[i][size-2] = '|';
		}

		int level = 2;
		while (!stk.empty()) {
			auto node = stk.back()->children[0]; stk.pop_back();
			while (node->id != INT32_MAX) {
				std::string s = std::to_string(node->id);
				int start = id_idx[node->id];
				buffer[level].replace(start, s.size(), s);
				buffer[level-1][start] = '|';
				node = node->children[0];
			}
			level += 2;
		}

		for (int i=bufferSize-1; i>=0; --i) {
			std::cout << buffer[i] << std::endl;
		}
	}

};