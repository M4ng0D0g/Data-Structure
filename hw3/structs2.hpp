// #include <map>
// #include <unordered_map>

// class IDS {
// public:
// 	virtual ~IDS() = default;
// 	virtual void Insert(int id, int score) = 0;
// 	virtual double SearchAVG(int id) = 0;
// };

// class BST : public IDS {
// private:
// 	std::multimap<int, int> data_;

// public:
// 	void Insert(int id, int score) override {
// 		data_.insert({id, score});
// 	}
// 	double SearchAVG(int id) override {
// 		auto er = data_.equal_range(id);

// 		for (int i)


// 		if (!data_[id].size()) return -1;

// 		double result = 0;
// 		for (const auto& s : data_[id]) {
// 			result += s;
// 		}
// 		return result / 10;
// 	}
// };

// class HT : public IDS {
// private:
// 	std::unordered_map<int, std::vector<int>> data_;

// public:
// 	void Insert(int id, int score) override {
// 		data_[id].push_back(score);
// 	}
// 	double SearchAVG(int id) override {
// 		if (!data_[id].size()) return -1;

// 		double result = 0;
// 		for (const auto& s : data_[id]) {
// 			result += s;
// 		}
// 		return result / 10;
// 	}
// };