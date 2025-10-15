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
}
```