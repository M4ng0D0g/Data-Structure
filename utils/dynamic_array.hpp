#pragma once
#include <vector>
#include <iostream>

template <typename T>
class DynamicArray {
private:
	std::vector<T> data_;
	size_t reallocFactor_; 

public:
	DynamicArray(size_t initialCapacity = 1, size_t reallocFactor = 10) : data_(), reallocFactor_(reallocFactor) {
		data_.reserve(initialCapacity);
	}

	void push_back(const T& value) {
		if (data_.size() >= data_.capacity()) {
			data_.reserve(data_.capacity() * reallocFactor_);
		}
		data_.push_back(value);
	}

	size_t size() const {
		return data_.size();
	}
	size_t capacity() const {
		return data_.capacity();
	}
	T& operator[](size_t idx) {
		return data_[idx];
	}
	const T& operator[](size_t idx) const {
		return data_[idx];
	}

	auto insert(typename std::vector<T>::iterator it, T v) {
		return data_.insert(it, v);
	}

	auto begin() { return data_.begin(); }
	auto end() { return data_.end(); }

	auto begin() const { return data_.cbegin(); }
	auto end() const { return data_.cend(); }
};