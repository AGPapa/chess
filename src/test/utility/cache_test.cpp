#include "../../utility/cache.cpp"

#include <gtest/gtest.h>

TEST(CacheTest, add) {
    std::unique_ptr<Cache<int, int>> cache = std::unique_ptr<Cache<int, int>>(new Cache<int, int>(4));
    cache->add(99, std::move(std::unique_ptr<int>(new int(1))));
    cache->add(98, std::move(std::unique_ptr<int>(new int(2))));
    cache->add(97, std::move(std::unique_ptr<int>(new int(3))));
    cache->add(96, std::move(std::unique_ptr<int>(new int(4))));

    ASSERT_EQ(2, *(cache->get(98)));
    ASSERT_EQ(1, *(cache->get(99)));
    ASSERT_EQ(3, *(cache->get(97)));
    ASSERT_EQ(4, *(cache->get(96)));

    cache->add(95, std::move(std::unique_ptr<int>(new int(5))));

    ASSERT_EQ(5, *(cache->get(95)));
    ASSERT_EQ(nullptr, cache->get(98)); // least recently accessed element is removed
    ASSERT_EQ(1, *(cache->get(99)));
    ASSERT_EQ(3, *(cache->get(97)));
    ASSERT_EQ(4, *(cache->get(96)));

}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}