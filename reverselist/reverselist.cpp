#include <iostream>
#include <stack>
#include "gtest/gtest.h"

template<typename T>
struct Node {
    Node(T val) : value(val), next(nullptr) {}

    T value;
    Node<T>* next;
};

template<typename T>
void ReverseList(Node<T>* root) {
    Node<T>* prev = nullptr;
    Node<T>* current = root;

    while (current) {
        Node<T>* next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
}

TEST(ReverseList, TestNull) {   
    ReverseList<int>(nullptr);
}

TEST(ReverseList, TestOneNode) {
    std::unique_ptr<Node<int>> n1(new Node<int>(10));

    ReverseList(n1.get());

    EXPECT_EQ(nullptr, n1->next);
}

TEST(ReverseList, TestReverse) {
    std::unique_ptr<Node<int>> n1(new Node<int>(10));
    std::unique_ptr<Node<int>> n2(new Node<int>(20));
    n1->next = n2.get();

    ReverseList(n1.get());

    EXPECT_EQ(nullptr, n1->next);
    EXPECT_EQ(n1.get(), n2->next);
}
