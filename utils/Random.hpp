#pragma once

#include <random>

class Random {
public:
    // 建構子：可以指定初始 seed
    explicit Random(unsigned int seed = std::random_device{}())
        : rng(seed), intDist(0, 100), doubleDist(0.0, 1.0) {}

    // 重設整數分布
    void setIntRange(int min, int max) {
        intDist = std::uniform_int_distribution<int>(min, max);
    }

    // 重設浮點分布
    void setDoubleRange(double min, double max) {
        doubleDist = std::uniform_real_distribution<double>(min, max);
    }

    // 取整數
    int nextInt() {
        return intDist(rng);
    }

    // 取浮點
    double nextDouble() {
        return doubleDist(rng);
    }

    // 重新設定 seed（本 instance 獨立）
    void reseed(unsigned int seed) {
        rng.seed(seed);
    }

private:
    std::mt19937 rng; // 每個 instance 自己的 engine
    std::uniform_int_distribution<int> intDist;
    std::uniform_real_distribution<double> doubleDist;
};
