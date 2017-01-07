#include <iostream>
#include <queue>
#include <stack>
#include <algorithm>
#include "gtest/gtest.h"

template <typename T>
void merge(
    std::vector<T>& items,
    int a_index,
    int a_size,
    int b_index,
    int b_size
) {
    int dest_index = a_index;
    
    std::deque<T> a(items.begin() + a_index, items.begin() + a_index + a_size);
    std::deque<T> b(items.begin() + b_index, items.begin() + b_index + b_size);

    while (!a.empty() && !b.empty()) {
        if (a.front() <= b.front()) {
            items[dest_index] = a.front();
            a.pop_front();
        } else {
            items[dest_index] = b.front();
            b.pop_front();
        }

        dest_index++;
    }

    while (!a.empty()) {
        items[dest_index] = a.front();
        a.pop_front();
        dest_index++;
    }

    while (!b.empty()) {
        items[dest_index] = b.front();
        b.pop_front();
        dest_index++;
    }
}

template <typename T>
void mergesort(std::vector<T>& items, int start_index, int items_to_sort) {
    if (items_to_sort <= 1) {
        return;
    }

    const int first_chunk_size = items_to_sort / 2;
    const int second_chunk_size = items_to_sort - first_chunk_size;
    const int b_index = start_index + first_chunk_size;
    mergesort(items, start_index, first_chunk_size);
    mergesort(items, b_index, second_chunk_size);

    merge(items, start_index, first_chunk_size, b_index, second_chunk_size);
}

template <typename T>
void mergesort_no_recursion(std::vector<T>& items_to_sort) {
    typedef std::pair<int, int> Range;

    auto Length = [] (Range r) { return r.second - r.first + 1; };

    for (int step = 1; step <= items_to_sort.size(); step *= 2) {
        for (int block = 0; block * step * 2 < items_to_sort.size(); block++) {
            int left_start = block * step * 2;
            int left_end = (left_start + step) - 1;
            if (left_end >= items_to_sort.size() - 1) {
                // No right block, so we can't merge here
                continue;
            }

            int right_start = left_end + 1;
            int right_end = (right_start + step) - 1;
            right_end = std::min(right_end, int(items_to_sort.size() - 1));

            Range left_range(left_start, left_end);
            Range right_range(right_start, right_end);
            merge(items_to_sort, left_range.first, Length(left_range), right_range.first, Length(right_range));
        }
    }
}

template <typename T>
void mergesort(std::vector<T>& items_to_sort) {
    mergesort(items_to_sort, 0, items_to_sort.size());
}

TEST(MergeSort, CheckEmpty) {
    std::vector<int> items = { };
    mergesort(items);
    EXPECT_EQ(0, items.size());
}

TEST(MergeSort, SortEven) {
    std::vector<int> items = { 6, 2, -100, 464 };
    mergesort(items);
    EXPECT_EQ(-100, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(6, items[2]);
    EXPECT_EQ(464, items[3]);
}

TEST(MergeSort, SortOdd) {
    std::vector<int> items = { 6, 2, -100, 464, 12 };
    mergesort(items);
    EXPECT_EQ(-100, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(6, items[2]);
    EXPECT_EQ(12, items[3]);
    EXPECT_EQ(464, items[4]);
}

TEST(MergeSort, AnotherTest) {
    std::vector<int> items = { 6, 5, 4, 3, 2, 1 };
    mergesort(items);
    EXPECT_EQ(1, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(3, items[2]);
    EXPECT_EQ(4, items[3]);
    EXPECT_EQ(5, items[4]);
    EXPECT_EQ(6, items[5]);
}

TEST(MergeSortNoRecursion, CheckEmpty) {
    std::vector<int> items = { };
    mergesort_no_recursion(items);
    EXPECT_EQ(0, items.size());
}

TEST(MergeSortNoRecursion, SortEven) {
    std::vector<int> items = { 6, 2, -100, 464 };
    mergesort_no_recursion(items);
    EXPECT_EQ(-100, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(6, items[2]);
    EXPECT_EQ(464, items[3]);
}

TEST(MergeSortNoRecursion, SortOdd) {
    std::vector<int> items = { 6, 2, -100, 464, 12 };
    mergesort_no_recursion(items);
    EXPECT_EQ(-100, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(6, items[2]);
    EXPECT_EQ(12, items[3]);
    EXPECT_EQ(464, items[4]);
}

TEST(MergeSortNoRecursion, AnotherTest) {
    std::vector<int> items = { 6, 5, 4, 3, 2, 1 };
    mergesort_no_recursion(items);
    EXPECT_EQ(1, items[0]);
    EXPECT_EQ(2, items[1]);
    EXPECT_EQ(3, items[2]);
    EXPECT_EQ(4, items[3]);
    EXPECT_EQ(5, items[4]);
    EXPECT_EQ(6, items[5]);
}
