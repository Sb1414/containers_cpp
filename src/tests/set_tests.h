#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <set>
#include <unordered_set>

#include "../s21_containers.h"

TEST(set, Constructor_Default) {
  s21::set<int> s21_set;
  std::set<int> std_set;
  EXPECT_EQ(s21_set.size(), std_set.size());
  EXPECT_EQ(s21_set.empty(), std_set.empty());
}

TEST(set, Constructor_Initializer_list) {
  s21::set<int> s21_set = {1, 7, 0, 3};
  std::set<int> std_set = {1, 7, 0, 3};
  EXPECT_EQ(s21_set.size(), std_set.size());
  EXPECT_EQ(s21_set.empty(), std_set.empty());
}

TEST(set, Constructor_Copy) {
  s21::set<char> set_orig = {'e', 'v', 'e', 't', 't', 'e', 'i', 's'};
  s21::set<char> set_copy = set_orig;
  EXPECT_EQ(set_orig.size(), set_copy.size());
  auto it1 = set_orig.begin();
  auto it2 = set_copy.begin();
  while (it1 != set_orig.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}

TEST(set, Constructor_Move) {
  s21::set<char> s21_orig = {'e', 'v', 'e', 't', 't', 'e', 'i', 's'};
  std::set<char> std_orig = {'e', 'v', 'e', 't', 't', 'e', 'i', 's'};
  s21::set<char> s21_moved(s21_orig);
  std::set<char> std_moved(std_orig);

  EXPECT_EQ(s21_moved.size(), std_moved.size());
  EXPECT_EQ(s21_orig.size(), std_orig.size());
  EXPECT_EQ(s21_orig.empty(), std_orig.empty());
  auto it1 = s21_moved.begin();
  auto it2 = std_moved.begin();
  while (it1 != s21_moved.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}

TEST(set, Empty_test) {
  s21::set<int> s21_set;
  std::set<int> std_set;
  EXPECT_EQ(s21_set.empty(), std_set.empty());
}

TEST(set, Modifier_Insert_1) {
  s21::set<char> set_1 = {'a', 'c', 'd'};
  s21::set<char> set_2 = {'a', 'c', 'd', 'b'};

  auto it = set_1.begin();
  ++it;
  set_1.insert('b');
  set_1.insert('a');
  auto it1 = set_1.begin();
  auto it2 = set_2.begin();
  while (it1 != set_1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
  EXPECT_EQ(set_1.size(), set_2.size());
}

TEST(set, Modifier_Insert_2) {
  s21::set<char> s21_set;
  s21_set.insert('g');
  s21_set.insert('g');
  EXPECT_EQ(s21_set.size(), size_t(1));
}

TEST(set, Modifier_Erase) {
  s21::set<char> s21_set_1 = {'a', 'c', 'd'};
  s21::set<char> s21_set_2 = {'a', 'b', 'c', 'd'};

  auto it = s21_set_2.begin();
  ++it;
  s21_set_2.erase(it);
  auto it1 = s21_set_1.begin();
  auto it2 = s21_set_2.begin();
  while (it1 != s21_set_1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
  EXPECT_EQ(s21_set_1.size(), s21_set_2.size());
}

TEST(test, Erase1) {
  std::initializer_list<int> items{1, 5, 3, 2};
  s21::set<int> my_set(items);
  std::set<int> std_set(items);
  auto my_iter = my_set.begin();
  auto std_iter = std_set.begin();
  ++std_iter;
  ++my_iter;
  ++std_iter;
  ++my_iter;
  std_set.erase(std_iter);
  my_set.erase(my_iter);
  my_iter = my_set.begin();
  std_iter = std_set.begin();
  while (std_iter != std_set.end()) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
}

TEST(test, Erase2) {
  std::initializer_list<int> items{1, 5, 3, 4};
  s21::set<int> my_set(items);
  std::set<int> std_set(items);
  auto my_iter = my_set.begin();
  auto std_iter = std_set.begin();
  ++std_iter;
  ++my_iter;
  ++std_iter;
  ++my_iter;
  std_set.erase(std_iter);
  my_set.erase(my_iter);
  my_iter = my_set.begin();
  std_iter = std_set.begin();
  while (std_iter != std_set.end()) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
}

TEST(test, Erase3) {
  std::initializer_list<int> items{1, 5, 7, 6};
  s21::set<int> my_set(items);
  std::set<int> std_set(items);
  auto my_iter = my_set.begin();
  auto std_iter = std_set.begin();
  ++std_iter;
  ++my_iter;
  ++std_iter;
  ++my_iter;
  ++std_iter;
  ++my_iter;
  std_set.erase(std_iter);
  my_set.erase(my_iter);
  my_iter = my_set.begin();
  std_iter = std_set.begin();
  while (std_iter != std_set.end()) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
}

TEST(test, Erase4) {
  std::initializer_list<int> items{1, 5, 7, 8};
  s21::set<int> my_set(items);
  std::set<int> std_set(items);
  auto my_iter = my_set.begin();
  auto std_iter = std_set.begin();
  ++std_iter;
  ++my_iter;
  ++std_iter;
  ++my_iter;
  std_set.erase(std_iter);
  my_set.erase(my_iter);
  my_iter = my_set.begin();
  std_iter = std_set.begin();
  while (std_iter != std_set.end()) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
}

TEST(test, Erase5) {
  std::initializer_list<int> items{1, 5, 7, 6};
  s21::set<int> my_set(items);
  std::set<int> std_set(items);
  auto my_iter = my_set.begin();
  auto std_iter = std_set.begin();
  std_set.erase(std_iter);
  my_set.erase(my_iter);
  my_iter = my_set.begin();
  std_iter = std_set.begin();
  while (std_iter != std_set.end()) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
}

TEST(test, Erase6) {
  std::initializer_list<int> items{5, 1, 7, 6};
  s21::set<int> my_set(items);
  std::set<int> std_set(items);
  auto my_iter = my_set.begin();
  auto std_iter = std_set.begin();
  std_set.erase(std_iter);
  my_set.erase(my_iter);
  my_iter = my_set.begin();
  std_iter = std_set.begin();
  while (std_iter != std_set.end()) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
}

TEST(test, Erase7) {
  std::initializer_list<int> items{5};
  s21::set<int> my_set(items);
  std::set<int> std_set(items);
  auto my_iter = my_set.begin();
  auto std_iter = std_set.begin();
  std_set.erase(std_iter);
  my_set.erase(my_iter);
  my_iter = my_set.begin();
  std_iter = std_set.begin();
  while (std_iter != std_set.end()) {
    ASSERT_TRUE(*my_iter == *std_iter);
    ++my_iter;
    ++std_iter;
  }
}

TEST(set, Modifier_Swap) {
  s21::set<char> s21_set_1 = {'a', 'b', 'o', 'b', 'a'};
  s21::set<char> s21_set_2 = {'s', 'h', 'l', 'e', 'p', 'p', 'a'};
  s21::set<char> s21_set_3 = {'a', 'b', 'o', 'b', 'a'};
  s21::set<char> s21_set_4 = {'s', 'h', 'l', 'e', 'p', 'p', 'a'};
  s21_set_1.swap(s21_set_2);
  auto it1 = s21_set_1.begin();
  auto it2 = s21_set_2.begin();
  auto it3 = s21_set_3.begin();
  auto it4 = s21_set_4.begin();
  while (it1 != s21_set_1.end()) {
    EXPECT_EQ(*it1, *it4);
    ++it1, ++it4;
  }
  while (it2 != s21_set_2.end()) {
    EXPECT_EQ(*it2, *it3);
    ++it2, ++it3;
  }
  EXPECT_EQ(s21_set_1.size(), s21_set_4.size());
  EXPECT_EQ(s21_set_2.size(), s21_set_3.size());
}

// ////////////////////////

TEST(set, swap) {
  s21::set<int> set1 = {1, 2, 3};
  s21::set<int> set2 = {4, 5, 6};

  ASSERT_EQ(*set1.begin(), 1);
  ASSERT_EQ(*set2.begin(), 4);

  set1.swap(set2);

  ASSERT_EQ(*set1.begin(), 4);
  ASSERT_EQ(*set2.begin(), 1);
}

TEST(set, Modifier_Merge) {
  s21::set<int> s21_set_1 = {1, 2, 3};
  s21::set<int> s21_set_2 = {3, 4, 5, 6};
  s21::set<int> s21_set_3 = {1, 2, 3, 4, 5, 6};
  s21_set_1.merge(s21_set_2);
  auto it1 = s21_set_1.begin();
  auto it3 = s21_set_3.begin();
  while (it1 != s21_set_1.end()) {
    EXPECT_EQ(*it1, *it3);
    ++it1, ++it3;
  }
  EXPECT_EQ(s21_set_1.size(), s21_set_3.size());
}

TEST(set, Lookup_Contains) {
  s21::set<int> s21_set;
  s21_set.insert(1);
  s21_set.insert(2);
  s21_set.insert(3);
  s21_set.insert(4);
  EXPECT_EQ(s21_set.contains(1), true);
  EXPECT_EQ(s21_set.contains(2), true);
  EXPECT_EQ(s21_set.contains(3), true);
  EXPECT_EQ(s21_set.contains(4), true);
  EXPECT_EQ(s21_set.contains(7), false);
  EXPECT_EQ(s21_set.contains(-2), false);
  EXPECT_EQ(s21_set.contains(0), false);
}

TEST(set, Lookup_Find) {
  s21::set<int> s21_set;
  s21_set.insert(1);
  s21_set.insert(2);
  s21_set.insert(3);
  s21_set.insert(4);
  auto it_end = s21_set.end();
  EXPECT_EQ(*(s21_set.find(1)), 1);
  EXPECT_EQ(*(s21_set.find(3)), 3);
  EXPECT_EQ(*(s21_set.find(4)), 4);
  EXPECT_EQ(*(s21_set.find(2)), 2);
  EXPECT_EQ(s21_set.find(-4), it_end);
  EXPECT_EQ(s21_set.find(0), it_end);
  EXPECT_EQ(s21_set.find(120), it_end);
}

TEST(set, Rehash_And_Insert_In_Collision) {
  s21::set<std::string> s21_set;
  std::unordered_set<std::string> std_set;
  s21_set.insert("asd");
  s21_set.insert("asa");
  s21_set.insert("asf");
  s21_set.insert("asg");
  s21_set.insert("ash");
  s21_set.insert("aqw");
  s21_set.insert("fgh");
  s21_set.insert("ghf");
  s21_set.insert("dghfg");
  s21_set.insert("dghfdsg");
  s21_set.insert("dghfgfsadfsd");
  s21_set.insert("dghfgfsadfsdasd");
  s21_set.insert("dghfgfsadfsasdasdd");
  s21_set.insert("dghfgfsadfsdasdasd");
  s21_set.insert("dghfgfdsasadfdsasasdsadsad");
  s21_set.insert("dghdsafgfsadfdsasasdsadsad");
  s21_set.insert("ådsdghfgfsadfdsasasdsadsad");
  s21_set.insert("dghfgfsadfdsasasdsaddsasad");
  s21_set.insert("dghfgfsadfdsasassadadsadsad");
  s21_set.insert("dghfgfsadfdsasdasasdsadsad");
  s21_set.insert("dghasdaasdfgfsadfsasdsadsad");
  s21_set.insert("asddghfgfsadfsasaß∂asdsadsad");
  s21_set.insert("dghfgfsadfasdsasdsadsad");
  s21_set.insert("dghfgfsadfsasdsadsad");
  s21_set.insert("dghfgfsasdadfsasdsadsad");
  s21_set.insert("asdsadadghfgfsadfsasdsadsad");
  std_set.insert("asd");
  std_set.insert("asa");
  std_set.insert("asf");
  std_set.insert("asg");
  std_set.insert("ash");
  std_set.insert("aqw");
  std_set.insert("fgh");
  std_set.insert("ghf");
  std_set.insert("dghfg");
  std_set.insert("dghfdsg");
  std_set.insert("dghfgfsadfsd");
  std_set.insert("dghfgfsadfsdasd");
  std_set.insert("dghfgfsadfsasdasdd");
  std_set.insert("dghfgfsadfsdasdasd");
  std_set.insert("dghfgfdsasadfdsasasdsadsad");
  std_set.insert("dghdsafgfsadfdsasasdsadsad");
  std_set.insert("ådsdghfgfsadfdsasasdsadsad");
  std_set.insert("dghfgfsadfdsasasdsaddsasad");
  std_set.insert("dghfgfsadfdsasassadadsadsad");
  std_set.insert("dghfgfsadfdsasdasasdsadsad");
  std_set.insert("dghasdaasdfgfsadfsasdsadsad");
  std_set.insert("asddghfgfsadfsasaß∂asdsadsad");
  std_set.insert("dghfgfsadfasdsasdsadsad");
  std_set.insert("dghfgfsadfsasdsadsad");
  std_set.insert("dghfgfsasdadfsasdsadsad");
  std_set.insert("asdsadadghfgfsadfsasdsadsad");
  for (auto i : std_set) {
    EXPECT_TRUE(s21_set.find(i) != s21_set.end());
  }
  EXPECT_EQ(s21_set.size(), std_set.size());
}