#include <iostream>
#include <stack>
#include "gtest/gtest.h"

bool IsBalanced(const std::string& expression, int& failure_index);

bool IsBalanced(const std::string& expression, int& failure_index) {
    std::stack<int> stack;

    for (int i = 0; i < expression.size(); i++) {
        char c = expression[i];

        if (c == '(') {
            stack.push(i);
        } else if (c == ')') {
            if (stack.empty()) {
                failure_index = i;
                return false;
            }

            stack.pop();
        }
    }

    if (stack.empty()) {
        return true;
    } else {
        while (!stack.empty()) {
            failure_index = stack.top();
            stack.pop();
        }
        return false;
    }
}

TEST(Brackets, BalancedExpressions) {
    int failure_index;
    EXPECT_TRUE(IsBalanced("(()())()", failure_index));
    EXPECT_TRUE(IsBalanced("", failure_index));
}

TEST(Brackets, TooManyClosing) {
    int failure_index;
    EXPECT_FALSE(IsBalanced("(())())()", failure_index));
    EXPECT_EQ(6, failure_index);

    EXPECT_FALSE(IsBalanced(")", failure_index));
    EXPECT_EQ(0, failure_index);

    EXPECT_FALSE(IsBalanced("())", failure_index));
    EXPECT_EQ(2, failure_index);
}

TEST(Brackets, TooManyOpening) {
    int failure_index;
    EXPECT_FALSE(IsBalanced("(", failure_index));
    EXPECT_EQ(0, failure_index);

    EXPECT_FALSE(IsBalanced("()(()(())", failure_index));
    EXPECT_EQ(2, failure_index);

    EXPECT_FALSE(IsBalanced("(()", failure_index));
    EXPECT_EQ(0, failure_index);

    EXPECT_FALSE(IsBalanced("(()())(((((((((", failure_index));
    EXPECT_EQ(6, failure_index);
}
