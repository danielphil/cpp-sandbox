#include <iostream>
#include "gtest/gtest.h"

template<typename T>
class HashSet {
private:
    const int m_hash_size = 100;
    std::vector<std::vector<T>> m_buckets;

    int CalculateHash(const T& value) const {
        return value % m_hash_size;
    }

public:
    HashSet() {
        m_buckets.resize(m_hash_size);
    }

    void Insert(const T& value) {
        std::vector<T>& bin = m_buckets[CalculateHash(value)];
        if (std::find(bin.begin(), bin.end(), value) == bin.end()) {
            bin.push_back(value);
        }
    }

    bool Contains(const T& value) const {
        const std::vector<T>& bin = m_buckets[CalculateHash(value)];
        return std::find(bin.begin(), bin.end(), value) != bin.end();
    }
};

TEST(HashSet, TestInsert) {
    HashSet<int> s;
    s.Insert(120);
    EXPECT_TRUE(s.Contains(120));
    EXPECT_FALSE(s.Contains(12));
}