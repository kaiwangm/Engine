#include <Engine/Runtime/Engine.h>
#include <gtest/gtest.h>
#include <ranges>

int add(int lhs, int rhs) { return lhs + rhs; }

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}

TEST(pipeline, simple)
{
    auto const ints   = {0, 1, 2, 3, 4, 5};
    auto       even   = [](int i) { return 0 == i % 2; };
    auto       square = [](int i) { return i * i; };

    for (int i : ints | std::views::filter(even) | std::views::transform(square))
    {
        // check that i is even
        EXPECT_EQ(0, i % 2);
        // check that i is a perfect square
        int const root = static_cast<int>(std::sqrt(i));
        EXPECT_EQ(i, root * root);
    }
}

TEST(pipeline, ok)
{
    auto const ints   = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    auto       even   = [](int i) { return 0 == i % 2; };
    auto       square = [](int i) { return i * i; };

    for (int i : ints | std::views::filter(even) | std::views::transform(square))
    {
        // check that i is even
        EXPECT_EQ(0, i % 2);
        // check that i is a perfect square
        int const root = static_cast<int>(std::sqrt(i));
        EXPECT_EQ(i, root * root);
    }
}

TEST(pipeline, hard)
{
    auto const ints   = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 100, 1000, 10000, 999999, 9999999, 99999999};
    auto       even   = [](int i) { return 0 == i % 2; };
    auto       square = [](int i) { return i * i; };

    for (int i : ints | std::views::filter(even) | std::views::transform(square))
    {
        // check that i is even
        EXPECT_EQ(0, i % 2);
        // check that i is a perfect square
        int const root = static_cast<int>(std::sqrt(i));
        EXPECT_EQ(i, root * root);
    }
}