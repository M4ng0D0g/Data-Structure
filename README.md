# Data-Structure

```cpp
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
```