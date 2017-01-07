#include <iostream>
#include <queue>
#include <stack>
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

    std::queue<Range> to_process;
    std::stack<Range> to_merge;

    to_process.push(Range(0, items_to_sort.size() - 1));

    while (!to_process.empty()) {
        Range current_range = to_process.front();
        to_process.pop();

        int left_range_length = Length(current_range) / 2;
        int right_range_length = (Length(current_range) + 1) / 2;

        Range left_range(current_range.first, current_range.first + left_range_length - 1);
        to_merge.push(left_range);

        Range right_range(left_range.second + 1, left_range.second + 1 + right_range_length - 1);
        to_merge.push(right_range);

        if (Length(left_range) > 1) {
            to_process.push(left_range);
        }

        if (Length(right_range) > 1) {
            to_process.push(right_range);
        }
    }

    while (!to_merge.empty()) {
        Range b = to_merge.top();
        to_merge.pop();

        Range a = to_merge.top();
        to_merge.pop();
        
        merge(items_to_sort, a.first, Length(a), b.first, Length(b));
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
    {
        typedef std::pair<int, int> Range;
        std::queue<Range> to_process;
        to_process.push(Range(10, 20));
        to_process.pop();
    }
    
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
