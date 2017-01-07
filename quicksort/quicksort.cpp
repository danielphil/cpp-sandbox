#include <iostream>
#include <queue>
#include <stack>
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

template <typename T>
void quicksort_no_recursion(std::vector<T>& items_to_sort) {
    if (items_to_sort.size() <= 1) {
        return;
    }
    
    typedef std::pair<int, int> Range;

    std::stack<Range> to_partition;
    to_partition.push(Range(0, items_to_sort.size()));

    while (!to_partition.empty()) {
        Range next = to_partition.top();
        to_partition.pop();

        int partition_index = partition(items_to_sort, next.first, next.second);
        
        int larger_length = next.second - 1 - partition_index;
        if (larger_length > 1) {
            to_partition.push(Range(partition_index + 1, next.second - 1 - partition_index));
        }

        int smaller_length = partition_index - next.first;
        if (smaller_length > 1) {
            to_partition.push(Range(next.first, partition_index - next.first));
        }
    }
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

TEST(QuickSortNoRecursion, CheckEmpty) {
    std::vector<int> items = { };
    quicksort_no_recursion(items);
    EXPECT_EQ(0, items.size());
}

TEST(QuickSortNoRecursion, SortEven) {
    std::vector<int> items = { 6, 2, -100, 464 };
    quicksort_no_recursion(items);
    EXPECT_EQ(-100, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(6, items[2]);
    EXPECT_EQ(464, items[3]);
}

TEST(QuickSortNoRecursion, SortOdd) {
    std::vector<int> items = { 6, 2, -100, 464, 12 };
    quicksort_no_recursion(items);
    EXPECT_EQ(-100, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(6, items[2]);
    EXPECT_EQ(12, items[3]);
    EXPECT_EQ(464, items[4]);
}

TEST(QuickSortNoRecursion, AnotherTest) {
    std::vector<int> items = { 6, 5, 4, 3, 2, 1 };
    quicksort_no_recursion(items);
    EXPECT_EQ(1, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(3, items[2]);
    EXPECT_EQ(4, items[3]);
    EXPECT_EQ(5, items[4]);
    EXPECT_EQ(6, items[5]);
}