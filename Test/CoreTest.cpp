#include <gtest/gtest.h>
#include <Engine.h>

int add(int lhs, int rhs) {
    return lhs + rhs; 
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    return 0;
}

TEST(sum, simple)
{
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
}

TEST(sum, ok)
{
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
}

TEST(sum, hard)
{
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
    EXPECT_EQ(add(2, 3), 5);
}