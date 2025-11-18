#pragma once
#include <vector>
#include <iostream>
#include <type_traits>
#include <limits>

namespace MyuDog::type {

	template <typename T>
	class NumberPlus {
		static_assert(std::is_integral<T>::value, "Only integral types supported.");

	private:
		size_t size_;
		std::vector<T> data_;
		const T MAX = std::numeric_limits<T>::max();
		const T MIN = std::numeric_limits<T>::min();

	public:
		NumberPlus& operator=(const NumberPlus& other) override {
			if (this == &other) return *this;
			size_ = other.size_;
			for (size_t i = 0; i < size_; ++i) {
				data_[i] = other.data_[i];
			}
			return *this;
		}

		NumberPlus operator+(const NumberPlus& other) override {
			bool carry = 0;
			for (size_t pos = 0; pos < size; ++pos) {
				if (other.data_[pos] > 0) {
					if (data_[pos] > (MAX - other.data_[pos] - carry)) {
						data_[pos] -= MAX - other.data_[pos] - carry;
						carry = 1;
					}
					else {
						data_[pos] += other.data_[pos] + carry;
						carry = 0;
					}
					
				}
				else {
					// *this < (std::numeric_limits<T>::min() - other);
				}
			}
		}
		NumberPlus operator+(T value) override {

		}
		
		NumberPlus operator-(const NumberPlus& other) override;
		NumberPlus operator*(const NumberPlus& other) override;
		NumberPlus operator/(const NumberPlus& other) override;
		NumberPlus operator%(const NumberPlus& other) override;
		NumberPlus& operator+=(const NumberPlus& other) override;
		NumberPlus& operator-=(const NumberPlus& other) override;
		NumberPlus& operator*=(const NumberPlus& other) override;
		NumberPlus& operator/=(const NumberPlus& other) override;
		NumberPlus& operator%=(const NumberPlus& other) override;

		// bool operator>(const NumberPlus& other) override;
		// bool operator>=(const NumberPlus& other) override;
		// bool operator<(const NumberPlus& other) override;
		// bool operator<=(const NumberPlus& other) override;
		// bool operator==(const NumberPlus& other) override;
		// bool operator!=(const NumberPlus& other) override;
		// bool operator&&(const NumberPlus& other) override;
		// bool operator||(const NumberPlus& other) override;

		// NumberPlus operator>>(const NumberPlus& other) override;
		// NumberPlus operator<<(const NumberPlus& other) override;
		// NumberPlus& operator>>=(const NumberPlus& other) override;
		// NumberPlus& operator<<=(const NumberPlus& other) override;

		// NumberPlus operator&(const NumberPlus& other) override;
		// NumberPlus operator|(const NumberPlus& other) override;
		// NumberPlus operator^(const NumberPlus& other) override;
		// NumberPlus operator!() override;

	};

}



