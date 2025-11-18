#pragma once
#include <memory>
#include <vector>
#include <array>

struct Node {
	int id, height;
	double priority;
	std::vector<int> scores;
	std::vector<Node*> children;
	Node(int id_) : id(id_) {}
};
using NodePtr = Node*;

class IDataStruct {
public:
	virtual ~IDataStruct() = default;

	virtual NodePtr Create(int id, int score) = 0;
	// 輸入：一個學生id與score。
	// 功能：生成僅包含id (key)與score (value)的資料結構，並return其 root。

	virtual NodePtr Insert(int id, int score) = 0;
	// 輸入：資料結構的 root 和一個(id, score) pair。
	// 功能：將(id, score)插入資料結構中，return更新後的 root。
	// 限制：AVL tree 跟 treap 只能使用”向上旋轉”

	virtual void Print() = 0;
	// 輸入：資料結構的 root。
	// 功能：列印資料結構中的所有節點資訊（格式不限）。

	virtual int Height() = 0;
	// 輸入：資料結構的 root。
	// 功能：輸出資料結構的高度。

	virtual double SearchAVG(int id) = 0;
	// 輸入：資料結構的 root與一個id。
	// 功能：輸出該id所有score的平均（如無資料，則輸出-1）。

};