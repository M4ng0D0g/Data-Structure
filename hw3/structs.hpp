#include <map>
#include <unordered_map>
#include <vector>

class IDS {
public:
	virtual ~IDS() = default;
	virtual void Insert(int id, int score) = 0;
	virtual double SearchAVG(int id) = 0;
};

class BST : public IDS {
private:
	std::map<int, std::vector<int>> data_;

public:
	void Insert(int id, int score) override {
		data_[id].push_back(score);
	}
	double SearchAVG(int id) override {
		double result = 0;
		int cnt = 0;
		if (!data_[id].size()) return -1;

		for (const auto& s : data_[id]) {
			result += s;
			++cnt;
		}
		return result / cnt;
	}
};

class HT : public IDS {
private:
	std::unordered_map<int, std::vector<int>> data_;

public:
	void Insert(int id, int score) override {
		data_[id].push_back(score);
	}
	double SearchAVG(int id) override {
		double result = 0;
		int cnt = 0;
		if (!data_[id].size()) return -1;

		for (const auto& s : data_[id]) {
			result += s;
			++cnt;
		}
		return result / cnt;
	}
};