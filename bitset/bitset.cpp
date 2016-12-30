#include <iostream>
#include <stack>
#include "gtest/gtest.h"

class Bitset
{
private:
    typedef unsigned int block_type;

public:
    Bitset(
        unsigned int max
    ) :
        m_bits_per_block(sizeof(block_type) * 8)
    {
        std::size_t blocks_required = (max + m_bits_per_block) / m_bits_per_block;
        m_blocks.resize(blocks_required, 0);
    }

    void Insert(unsigned int value) {
        std::size_t block_index = value / m_bits_per_block;
        unsigned int bit_index = value % m_bits_per_block;

        auto& block = m_blocks.at(block_index);
        block |= 1 << bit_index;
    }

    void Remove(unsigned int value) {
        std::size_t block_index = value / m_bits_per_block;
        unsigned int bit_index = value % m_bits_per_block;

        auto& block = m_blocks.at(block_index);
        block &= ~(1 << bit_index);
    }

    bool IsSet(unsigned int value) {
        std::size_t block_index = value / m_bits_per_block;
        unsigned int bit_index = value % m_bits_per_block;

        const auto& block = m_blocks.at(block_index);
        return (block & (1 << bit_index)) != 0;
    }

private:
    std::vector<block_type> m_blocks;
    const std::size_t m_bits_per_block;
};

TEST(Bitset, CheckEmptyOnCreation) {
    Bitset b(100);
    for (int i = 0; i < 100; i++) {
        EXPECT_FALSE(b.IsSet(i));
    }
}

TEST(Bitset, CheckAddSetRemove) {
    Bitset b(200);

    b.Insert(63);
    for (int i = 0; i < 63; i++) {
        EXPECT_FALSE(b.IsSet(i));
    }
    EXPECT_TRUE(b.IsSet(63));
    for (int i = 64; i < 200; i++) {
        EXPECT_FALSE(b.IsSet(i));
    }

    b.Insert(63);
    EXPECT_TRUE(b.IsSet(63));

    b.Remove(63);
    b.Remove(1);
    for (int i = 0; i < 200; i++) {
        EXPECT_FALSE(b.IsSet(i)) << i;
    }
}
