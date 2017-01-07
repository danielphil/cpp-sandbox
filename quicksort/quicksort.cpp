#include <iostream>
#include <queue>
#include "gtest/gtest.h"

template <typename T>
int partition(std::vector<T>& items, int start_index, int length) {
    const int end_index = start_index + length - 1;
    int partition_index = end_index;
    int larger_index = start_index;

    for (int i = start_index; i < end_index; i++) {
        if (items[i] < items[partition_index]) {
            std::swap(items[i], items[larger_index]);
            larger_index++;
        }
    }

    std::swap(items[partition_index], items[larger_index]);

    return larger_index;
}

template <typename T>
void quicksort(std::vector<T>& items, int start_index, int length) {
    if (length <= 1) {
        return;
    }

    int partition_index = partition(items, start_index, length);
    quicksort(items, start_index, partition_index - start_index);
    quicksort(items, partition_index + 1, length - 1 - partition_index);
}

template <typename T>
void quicksort(std::vector<T>& items_to_sort) {
    quicksort(items_to_sort, 0, items_to_sort.size());
}

TEST(QuickSort, CheckEmpty) {
    std::vector<int> items = { };
    quicksort(items);
    EXPECT_EQ(0, items.size());
}

TEST(QuickSort, SortEven) {
    std::vector<int> items = { 6, 2, -100, 464 };
    quicksort(items);
    EXPECT_EQ(-100, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(6, items[2]);
    EXPECT_EQ(464, items[3]);
}

TEST(QuickSort, SortOdd) {
    std::vector<int> items = { 6, 2, -100, 464, 12 };
    quicksort(items);
    EXPECT_EQ(-100, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(6, items[2]);
    EXPECT_EQ(12, items[3]);
    EXPECT_EQ(464, items[4]);
}

TEST(QuickSort, AnotherTest) {
    std::vector<int> items = { 6, 5, 4, 3, 2, 1 };
    quicksort(items);
    EXPECT_EQ(1, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(3, items[2]);
    EXPECT_EQ(4, items[3]);
    EXPECT_EQ(5, items[4]);
    EXPECT_EQ(6, items[5]);
}