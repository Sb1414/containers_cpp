#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <unordered_set>

#include "../s21_containersplus.h"

TEST(MultisetTest, TestDefaultConstructor) {
  s21::multiset<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(MultisetTest, TestInitializerListConstructor) {
  s21::multiset<int> s = {3, 2, 1, 1, 2, 3};
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 6);
  EXPECT_EQ(s.count(1), 2);
  EXPECT_EQ(s.count(2), 2);
  EXPECT_EQ(s.count(3), 2);
}

TEST(MultisetTest, TestCopyConstructor) {
  s21::multiset<int> s1 = {1, 2, 3};
  s21::multiset<int> s2(s1);
  EXPECT_FALSE(s1.empty());
  EXPECT_FALSE(s2.empty());
  EXPECT_EQ(s1.size(), s2.size());
  EXPECT_EQ(s1.count(1), s2.count(1));
  EXPECT_EQ(s1.count(2), s2.count(2));
  EXPECT_EQ(s1.count(3), s2.count(3));
}

TEST(MultisetTest, TestMoveConstructor) {
  s21::multiset<int> s1 = {1, 2, 3};
  s21::multiset<int> s2(std::move(s1));
  EXPECT_TRUE(s1.empty());
  EXPECT_FALSE(s2.empty());
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(s2.count(1), 1);
  EXPECT_EQ(s2.count(2), 1);
  EXPECT_EQ(s2.count(3), 1);
}

TEST(MultisetTest, TestCopyAssignment) {
  s21::multiset<int> s1 = {1, 2, 3};
  s21::multiset<int> s2 = {4, 5, 6};
  s1 = s2;
  EXPECT_FALSE(s1.empty());
  EXPECT_FALSE(s2.empty());
  EXPECT_EQ(s1.size(), s2.size());
  EXPECT_EQ(s1.count(4), s2.count(4));
  EXPECT_EQ(s1.count(5), s2.count(5));
  EXPECT_EQ(s1.count(6), s2.count(6));
}

TEST(MultisetTest, TestInsertEmpty) {
  s21::multiset<int> s;
  s.insert(1);
  EXPECT_EQ(s.size(), 1);
  EXPECT_EQ(s.count(1), 1);
}

TEST(MultisetTest, TestInsert1) {
  s21::multiset<int> s = {1, 2, 3};
  s.insert(2);
  s.insert(4);
  EXPECT_EQ(s.size(), 5);
  EXPECT_EQ(s.count(1), 1);
  EXPECT_EQ(s.count(2), 2);
  EXPECT_EQ(s.count(3), 1);
  EXPECT_EQ(s.count(4), 1);
}

TEST(MultisetTest, TestEmplaceBackEmpty) {
  s21::multiset<std::string> s;
  s.emplace("hsiuayes");
  s.emplace("bryceaet");
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.count("hsiuayes"), 1);
  EXPECT_EQ(s.count("bryceaet"), 1);
}

TEST(MultisetTest, TestEmplaceBack) {
  s21::multiset<std::string> s = {"evetteis"};
  s.emplace("hsiuayes");
  s.emplace("bryceaet");
  s.emplace("hsiuayes");
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.count("evetteis"), 1);
  EXPECT_EQ(s.count("hsiuayes"), 2);
  EXPECT_EQ(s.count("bryceaet"), 1);
}

TEST(MultisetTest, TestEmplaceEmpty) {
  s21::multiset<int> s;
  s.emplace(1);
  s.emplace(2);
  EXPECT_EQ(s.size(), 2);
  EXPECT_EQ(s.count(1), 1);
  EXPECT_EQ(s.count(2), 1);
}

TEST(MultisetTest, TestEmplace) {
  s21::multiset<std::string> s = {"evetteis", "bryceaet"};
  s.emplace("hsiuayes");
  s.emplace("bryceaet");
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.count("evetteis"), 1);
  EXPECT_EQ(s.count("hsiuayes"), 1);
  EXPECT_EQ(s.count("bryceaet"), 2);
}

TEST(MultisetTest, TestClearEmpty) {
  s21::multiset<int> s;
  s.clear();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(MultisetTest, TestClear) {
  s21::multiset<int> s = {1, 2, 3};
  s.clear();
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(MultisetTest, TestBeginEmpty) {
  s21::multiset<int> s;
  auto it = s.begin();
  EXPECT_EQ(it, s.end());
}

TEST(MultisetTest, TestBegin) {
  s21::multiset<int> s = {1, 2, 3};
  auto it = s.begin();
  EXPECT_EQ(*it, 1);
}

TEST(MultisetTest, TestMaxSize) {
  s21::multiset<int> s;
  auto max_size = s.max_size();
  EXPECT_GT(max_size, 0);
}

TEST(MultisetTest, TestMove) {
  s21::multiset<int> s1 = {1, 2, 3};
  s21::multiset<int> s2 = {4, 5, 6};
  s1 = std::move(s2);
  EXPECT_FALSE(s1.empty());
  EXPECT_EQ(s1.size(), 3);
  EXPECT_EQ(s1.count(4), 1);
  EXPECT_EQ(s1.count(5), 1);
  EXPECT_EQ(s1.count(6), 1);
}

TEST(MultisetTest, TestCopy) {
  s21::multiset<int> s1 = {1, 2, 3, 4};
  s21::multiset<int> s2 = s1;
  EXPECT_FALSE(s1.empty());
  EXPECT_FALSE(s2.empty());
  EXPECT_EQ(s1.size(), s2.size());
  EXPECT_EQ(s1.count(1), s2.count(1));
  EXPECT_EQ(s1.count(2), s2.count(2));
  EXPECT_EQ(s1.count(3), s2.count(3));
  EXPECT_EQ(s1.count(4), s2.count(4));
}

TEST(MultisetTest, TestEraseEmpty) {
  s21::multiset<int> s;
  EXPECT_NO_THROW(s.erase(s.begin()));
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(MultisetTest, TestErase) {
  s21::multiset<int> s = {1, 2, 3, 2, 4};
  auto it = s.find(2);
  EXPECT_NO_THROW(s.erase(it));
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.count(2), 1);
}

TEST(MultisetTest, TestSwap) {
  s21::multiset<int> s1 = {1, 2, 3};
  s21::multiset<int> s2 = {4, 5, 6};
  s1.swap(s2);
  EXPECT_FALSE(s1.empty());
  EXPECT_FALSE(s2.empty());
  EXPECT_EQ(s1.size(), 3);
  EXPECT_EQ(s1.count(4), 1);
  EXPECT_EQ(s1.count(5), 1);
  EXPECT_EQ(s1.count(6), 1);
  EXPECT_EQ(s2.size(), 3);
  EXPECT_EQ(s2.count(1), 1);
  EXPECT_EQ(s2.count(2), 1);
  EXPECT_EQ(s2.count(3), 1);
}

TEST(MultisetTest, TestMerge) {
  s21::multiset<int> s1 = {1, 2, 3, 3};
  s21::multiset<int> s2 = {2, 3, 4, 5};
  s1.merge(s2);
  EXPECT_FALSE(s1.empty());
  EXPECT_EQ(s1.size(), 8);
  EXPECT_EQ(s1.count(1), 1);
  EXPECT_EQ(s1.count(2), 2);
  EXPECT_EQ(s1.count(3), 3);
  EXPECT_EQ(s1.count(4), 1);
  EXPECT_EQ(s1.count(5), 1);
  EXPECT_TRUE(s2.empty());
}

TEST(MultisetTest, TestInsert2) {
  s21::multiset<int> s = {1, 2};
  s.insert(3);
  auto it = s.find(3);
  EXPECT_FALSE(s.empty());
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s.count(1), 1);
  EXPECT_EQ(s.count(2), 1);
  EXPECT_EQ(s.count(3), 1);
  EXPECT_TRUE(it != s.end());
  it = s.find(10);
  EXPECT_TRUE(it == s.end());
}

TEST(MultisetTest, EqualRange) {
  std::initializer_list<int> items{1, 2, 2, 3, 3, 3, 3, 3, 3, 5, 2, 35, 463, 2};
  s21::multiset<int> s21_(items);
  std::multiset<int> std_(items);

  std::pair<s21::multiset<int>::iterator, s21::multiset<int>::iterator>
      my_pair = s21_.equal_range(2);
  std::pair<std::multiset<int>::iterator, std::multiset<int>::iterator>
      std_pair = std_.equal_range(2);

  auto my_iter = my_pair.first;
  auto std_iter = std_pair.first;

  while (my_iter != my_pair.second) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
  ASSERT_TRUE(*my_iter == *std_iter);

  my_iter = my_pair.first;
  std_iter = std_pair.first;

  while (std_iter != std_pair.second) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
  ASSERT_TRUE(*my_iter == *std_iter);
}

TEST(MultisetTest, EqualRangeConst) {
  std::initializer_list<int> items{1, 2, 2, 3, 3, 3, 3, 3, 3, 5, 2, 35, 463, 2};
  s21::multiset<int> s21_(items);
  std::multiset<int> std_(items);

  auto const it = 2;
  std::pair<s21::multiset<int>::const_iterator,
            s21::multiset<int>::const_iterator>
      my_pair = s21_.equal_range(it);
  std::pair<std::multiset<int>::const_iterator,
            std::multiset<int>::const_iterator>
      std_pair = std_.equal_range(it);

  auto my_iter = my_pair.first;
  auto std_iter = std_pair.first;

  while (my_iter != my_pair.second) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
  ASSERT_TRUE(*my_iter == *std_iter);

  my_iter = my_pair.first;
  std_iter = std_pair.first;

  while (std_iter != std_pair.second) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
  ASSERT_TRUE(*my_iter == *std_iter);
}

TEST(MultisetTest, Contains) {
  std::initializer_list<int> items{14, 15, 0, 10, 11, 12, 18};
  s21::multiset<int> s21_(items);
  std::multiset<int> std_(items);
  bool my_cont = s21_.contains(14);
  ASSERT_TRUE(my_cont == true);
  my_cont = s21_.contains(15);
  ASSERT_TRUE(my_cont == true);
  my_cont = s21_.contains(0);
  ASSERT_TRUE(my_cont == true);
  my_cont = s21_.contains(18);
  ASSERT_TRUE(my_cont == true);
  my_cont = s21_.contains(-9);
  ASSERT_TRUE(my_cont == false);
}

TEST(MultisetTest, LowerBound) {
  std::initializer_list<int> items{10, 50, 30, 30, 60, 20, 23, 40};
  s21::multiset<int> s21_(items);
  std::multiset<int> std_(items);

  auto my_iter = s21_.lower_bound(30);
  auto std_iter = std_.lower_bound(30);
  for (int i = 1; i < 61; i++) {
    my_iter = s21_.lower_bound(i);
    std_iter = std_.lower_bound(i);
    ASSERT_TRUE(*my_iter == *std_iter);
  }
}

TEST(MultisetTest, UpperBound) {
  std::initializer_list<int> items{10, 50, 30, 30, 60, 20, 23, 40};
  s21::multiset<int> s21_(items);
  std::multiset<int> std_(items);

  auto my_iter = s21_.upper_bound(30);
  auto std_iter = std_.upper_bound(30);
  for (int i = 1; i < 60; i++) {
    my_iter = s21_.upper_bound(i);
    std_iter = std_.upper_bound(i);
    ASSERT_TRUE(*my_iter == *std_iter);
  }
}

TEST(MultisetTest, MultisetEmplace) {
  std::initializer_list<int> items{1, 2, 3};
  std::initializer_list<int> items2{2, 3, 543};
  s21::multiset<int> s21_(items);
  std::multiset<int> std_(items);
  std::multiset<int> std_multiset2(items2);
  std_.merge(std_multiset2);
  s21_.emplace(2, 3, 543);
  auto my_iter = s21_.begin();
  auto std_iter = std_.begin();
  while (std_iter != std_.end()) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
}