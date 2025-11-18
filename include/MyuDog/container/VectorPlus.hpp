#pragma once
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>

namespace MyuDog::container {

	template <typename T>
	class VectorPlus {
	private:
		size_t size_;
		size_t capacity_;
		std::unordered_map<size_t, std::shared_ptr<T>> data_;

		std::vector<size_t> idxList_;
		/*
		假設表 = vector(size, 1);
		
		1. 修改範圍 O(1)
		2. 取得 O(1)


		取得idx方式:
			使用時:
				1. idxList_[idx]直接取得
				2. 用區間和計算

		insert方式:


		push_back方式:
		remove方式:
		swap方式:


		
		*/
		

	public:
		T& operator[](size_t idx) override {
			if (idx >= size_) throw std::range_error("index out of range");
			return data_.at(idxList_[idx]);
		}

		void insert(size_t idx, T value) {

		}

		void push_back(const T& v) {
			/*
			
			
			*/
		}

		void erase() {

		}

	};
}