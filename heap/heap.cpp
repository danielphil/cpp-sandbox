#include <iostream>
#include "gtest/gtest.h"

class MinHeap
{
public:
    void Insert(int value);
    void Insert(const std::initializer_list<int>& items);
    int Min() const;
    int ExtractMinimum();
    bool IsEmpty();

private:
    bool HasParent(std::size_t index) const;
    std::size_t ParentIndex(std::size_t current_index) const;
    std::size_t LeftChildIndex(std::size_t parent_index) const;
    void BubbleUp(std::size_t index);
    void BubbleDown(std::size_t index);

    std::vector<int> m_heap;
};

bool MinHeap::IsEmpty() {
    return m_heap.empty();    
}

int MinHeap::ExtractMinimum() {
    int minimum = m_heap.at(0);

    m_heap[0] = m_heap.back();
    m_heap.pop_back();

    if (!m_heap.empty()) {
        BubbleDown(0);
    }

    return minimum;
}

void MinHeap::Insert(int value) {
    m_heap.push_back(value);
    BubbleUp(m_heap.size() - 1);
}

void MinHeap::Insert(const std::initializer_list<int>& items) {
    m_heap = items;

    for (std::size_t i = m_heap.size() / 2; i < m_heap.size(); i++) {
        BubbleDown(m_heap.size() - 1 - i);
    }
}

int MinHeap::Min() const {
    return m_heap.at(0);
}

void MinHeap::BubbleDown(std::size_t index) {
    int minimum_value = m_heap[index];
    int minimum_index = index;

    std::size_t child_index = LeftChildIndex(index);
    for (auto i = 0; i < 2; i++) {
        int c = child_index + i;
        if (c < m_heap.size() && m_heap[c] < minimum_value) {
            minimum_value = m_heap[c];
            minimum_index = c;
        }    
    }

    if (minimum_index != index) {
        std::swap(m_heap[index], m_heap[minimum_index]);
        BubbleDown(minimum_index);
    }
}

void MinHeap::BubbleUp(std::size_t index) {
    if (!HasParent(index)) {
        return;
    }

    auto parent_index = ParentIndex(index);
    if (m_heap[parent_index] > m_heap[index]) {
        std::swap(m_heap[parent_index], m_heap[index]);
        BubbleUp(parent_index);
    }
}

bool MinHeap::HasParent(std::size_t index) const {
    return index != 0;
}

std::size_t MinHeap::ParentIndex(std::size_t index) const {
    return ((index + 1) / 2) - 1;
}

std::size_t MinHeap::LeftChildIndex(std::size_t parent_index) const {
    return ((parent_index + 1) * 2) - 1;
}

TEST(MinHeap, CheckEmpty) {
    MinHeap heap;
    EXPECT_TRUE(heap.IsEmpty());

    heap.Insert(10);
    EXPECT_FALSE(heap.IsEmpty());
}

TEST(MinHeap, AddSingleItem) {   
    MinHeap heap;
    heap.Insert(5);
    EXPECT_EQ(5, heap.Min());
}

TEST(MinHeap, AddMultipleItems) {   
    MinHeap heap;
    heap.Insert({ 5, 2, -10, 100});
    EXPECT_EQ(-10, heap.Min());
}

TEST(MinHeap, ExtractMin) {   
    MinHeap heap;
    heap.Insert({ 5, 2, -10, 100, -55, 72, 555});
    EXPECT_EQ(-55, heap.ExtractMinimum());
    EXPECT_EQ(-10, heap.ExtractMinimum());
    EXPECT_EQ(2, heap.ExtractMinimum());
    EXPECT_EQ(5, heap.ExtractMinimum());
    EXPECT_EQ(72, heap.ExtractMinimum());
    EXPECT_EQ(100, heap.ExtractMinimum());
    EXPECT_EQ(555, heap.ExtractMinimum());
}