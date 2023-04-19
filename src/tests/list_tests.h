#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <list>

#include "../s21_containers.h"
using ::testing::ElementsAre;

TEST(List, Constructor_Default) {
  s21::list<int> s21_l;
  std::list<int> std_l;
  EXPECT_EQ(s21_l.size(), std_l.size());
  EXPECT_EQ(s21_l.empty(), std_l.empty());
}

TEST(List, Constructor_Initializer_list) {
  s21::list<int> s21_l = {1, 2, 3, 4};
  std::list<int> std_l = {1, 2, 3, 4};
  EXPECT_EQ(s21_l.front(), std_l.front());
  EXPECT_EQ(s21_l.back(), std_l.back());
  EXPECT_EQ(s21_l.size(), std_l.size());
  EXPECT_EQ(s21_l.empty(), std_l.empty());
}

TEST(List, Constructor_Parameterized) {
  s21::list<double> s21_l(5);
  std::list<double> std_l(5);
  EXPECT_EQ(s21_l.size(), std_l.size());
  EXPECT_EQ(s21_l.empty(), std_l.empty());
}
TEST(List, Constructor_Copy) {
  s21::list<char> s21_l_1 = {'h', 's', 'i', 'u', 'a', 'y', 'e', 's'};
  std::list<char> std_l_1 = {'h', 's', 'i', 'u', 'a', 'y', 'e', 's'};
  s21::list<char> s21_l_2 = s21_l_1;
  std::list<char> std_l_2 = std_l_1;

  EXPECT_EQ(s21_l_1.size(), s21_l_2.size());
  EXPECT_EQ(std_l_1.size(), std_l_2.size());
  auto it1 = s21_l_1.begin();
  auto it2 = s21_l_2.begin();
  while (it1 != s21_l_1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}
// TEST(List, Constructor_Move) {
//   s21::list<char> s21_l_1 = {'a', 'b', 'c', 'd'};
//   std::list<char> std_l_1 = {'a', 'b', 'c', 'd'};
//   s21::list<char> s21_l_2 = std::move(s21_l_1);
//   std::list<char> std_l_2 = std::move(std_l_1);

//   EXPECT_EQ(s21_l_2.size(), std_l_2.size());
//   EXPECT_EQ(s21_l_1.size(), std_l_1.size());
//   EXPECT_EQ(s21_l_1.empty(), std_l_1.empty());
//   auto it1 = s21_l_2.begin();
//   auto it2 = std_l_2.begin();
//   while (it1 != s21_l_2.end()) {
//     EXPECT_EQ(*it1, *it2);
//     ++it1, ++it2;
//   }
// }

TEST(ListConstructor, MoveConstructor) {
  s21::list<int> other_list = {1, 2, 3};
  s21::list<int> my_list(std::move(other_list));

  ASSERT_EQ(my_list.size(), 3);
  ASSERT_TRUE(other_list.empty());

  auto it = my_list.begin();
  EXPECT_EQ(*it++, 1);
  EXPECT_EQ(*it++, 2);
  EXPECT_EQ(*it, 3);
}

TEST(List, OperatorEq_test) {
  s21::list<int> l1{1, 2, 3, 4, 5};
  s21::list<int> l2{6, 7, 8, 9, 10};
  l1 = l2;
  EXPECT_EQ(l1.size(), l2.size());
  EXPECT_EQ(l1.empty(), l2.empty());
  auto it1 = l1.begin();
  auto it2 = l2.begin();
  for (; it1 != l1.end() && it2 != l2.end(); ++it1, ++it2) {
    EXPECT_EQ(*it1, *it2);
  }
}

TEST(ListTest, OperatorEq2_test) {
  s21::list<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  s21::list<int> list2;
  list2 = list1;

  ASSERT_EQ(list2.size(), 3);
  ASSERT_EQ(list2.front(), 1);
  ASSERT_EQ(list2.back(), 3);

  list1.clear();
  ASSERT_EQ(list2.size(), 3);
}

TEST(ListTest, AssignmentOperator) {
  s21::list<int> list1;
  list1.push_back(1);
  list1.push_back(2);
  list1.push_back(3);

  s21::list<int> &result = list1 = list1;

  ASSERT_EQ(&result, &list1);
  ASSERT_EQ(list1.size(), 3);
  ASSERT_EQ(list1.front(), 1);
  ASSERT_EQ(list1.back(), 3);
}

TEST(ListTest, MaxSize) {
  s21::list<int> l;
  ASSERT_LE(l.size(), l.max_size());
}

TEST(List, Iterator) {
  s21::list<char> s21_l_1 = {'a', 'b', 'c', 'd'};
  std::list<char> std_l_1 = {'a', 'b', 'c', 'd'};
  auto it1 = s21_l_1.begin();
  auto it2 = std_l_1.begin();
  while (it1 != s21_l_1.end()) {
    EXPECT_EQ(*it1, *it2);
    it1++, it2++;
  }
}

TEST(List, Modifier_Insert) {
  s21::list<int> s21_l_1 = {'a', 'c', 'd'};
  s21::list<int> s21_l_2 = {'a', 'b', 'c', 'd'};
  auto it = s21_l_1.begin();
  ++it;
  s21_l_1.insert(it, 'b');
  auto it1 = s21_l_1.begin();
  auto it2 = s21_l_2.begin();
  while (it1 != s21_l_1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
  EXPECT_EQ(s21_l_1.size(), s21_l_2.size());
}

TEST(List, Modifier_Erase) {
  s21::list<int> s21_l_1 = {'a', 'c', 'd'};
  s21::list<int> s21_l_2 = {'a', 'b', 'c', 'd'};

  auto it = s21_l_2.begin();
  ++it;
  s21_l_2.erase(it);
  auto it1 = s21_l_1.begin();
  auto it2 = s21_l_2.begin();
  while (it1 != s21_l_1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
  EXPECT_EQ(s21_l_1.size(), s21_l_2.size());
}

TEST(List, Modifier_Push) {
  s21::list<int> s21_l = {1, 2, 3, 4};
  std::list<int> std_l = {1, 2, 3, 4};
  s21_l.push_back(5);
  s21_l.push_front(0);
  std_l.push_back(5);
  std_l.push_front(0);
  EXPECT_EQ(s21_l.front(), std_l.front());
  EXPECT_EQ(s21_l.back(), std_l.back());
  EXPECT_EQ(s21_l.size(), std_l.size());
  EXPECT_EQ(s21_l.empty(), std_l.empty());
  auto it1 = s21_l.begin();
  auto it2 = std_l.begin();
  while (it1 != s21_l.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}

TEST(List, Modifier_Pop) {
  s21::list<int> s21_l = {1, 2, 3, 4};
  std::list<int> std_l = {1, 2, 3, 4};
  s21_l.pop_back();
  s21_l.pop_front();
  std_l.pop_back();
  std_l.pop_front();
  EXPECT_EQ(s21_l.front(), std_l.front());
  EXPECT_EQ(s21_l.back(), std_l.back());
  EXPECT_EQ(s21_l.size(), std_l.size());
  EXPECT_EQ(s21_l.empty(), std_l.empty());
  auto it1 = s21_l.begin();
  auto it2 = std_l.begin();
  while (it1 != s21_l.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}

TEST(List, Modifier_Swap) {
  s21::list<char> s21_l_1 = {'E', 'v', 'e', 't', 't', 'e', 'I',
                             's', 'a', 'd', 'o', 'r', 'a'};
  s21::list<char> s21_l_2 = {'B', 'r', 'y', 'c', 'e', 'A',
                             'e', 't', 'h', 'e', 'r'};
  s21::list<char> s21_l_3 = {'E', 'v', 'e', 't', 't', 'e', 'I',
                             's', 'a', 'd', 'o', 'r', 'a'};
  s21::list<char> s21_l_4 = {'B', 'r', 'y', 'c', 'e', 'A',
                             'e', 't', 'h', 'e', 'r'};
  s21_l_1.swap(s21_l_2);
  auto it1 = s21_l_1.begin();
  auto it2 = s21_l_2.begin();
  auto it3 = s21_l_3.begin();
  auto it4 = s21_l_4.begin();
  while (it1 != s21_l_1.end()) {
    EXPECT_EQ(*it1, *it4);
    ++it1, ++it4;
  }
  while (it2 != s21_l_2.end()) {
    EXPECT_EQ(*it2, *it3);
    ++it2, ++it3;
  }
  EXPECT_EQ(s21_l_1.size(), s21_l_4.size());
  EXPECT_EQ(s21_l_2.size(), s21_l_3.size());
}

TEST(List, Modifier_Merge) {
  s21::list<int> s21_l_1 = {1, 3, 7};
  s21::list<int> s21_l_2 = {-1, 0, 5, 8};
  std::list<int> std_l_1 = {1, 3, 7};
  std::list<int> std_l_2 = {-1, 0, 5, 8};
  s21_l_1.merge(s21_l_2);
  std_l_1.merge(std_l_2);
  auto it1 = s21_l_1.begin();
  auto it2 = std_l_1.begin();
  while (it1 != s21_l_1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
  EXPECT_EQ(s21_l_1.size(), std_l_1.size());
}

TEST(ListTest, MergeTest) {
  s21::list<int> my_list = {1, 3, 5, 7};
  s21::list<int> other_list = {2, 4, 6, 8};
  my_list.merge(other_list);
  EXPECT_THAT(my_list, ElementsAre(1, 2, 3, 4, 5, 6, 7, 8));
  EXPECT_TRUE(other_list.empty());

  my_list = {1, 3, 5, 7};
  other_list = {2, 2, 4, 6, 8};
  my_list.merge(other_list);
  EXPECT_THAT(my_list, ElementsAre(1, 2, 2, 3, 4, 5, 6, 7, 8));
  EXPECT_TRUE(other_list.empty());

  my_list = {1, 3, 5, 7};
  other_list = {};
  my_list.merge(other_list);
  EXPECT_THAT(my_list, ElementsAre(1, 3, 5, 7));
  EXPECT_TRUE(other_list.empty());
}

TEST(List, Modifier_Reverse) {
  s21::list<int> s21_l_1 = {1, 2, 3, 4};
  s21::list<int> s21_l_2 = {4, 3, 2, 1};
  s21_l_1.reverse();
  auto it1 = s21_l_1.begin();
  auto it2 = s21_l_2.begin();
  while (it1 != s21_l_1.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
  EXPECT_EQ(s21_l_1.size(), s21_l_2.size());
}

TEST(List, Modifier_Unique) {
  s21::list<int> s21_l{1, -1, -23, 4, 1, 5, 6, 4, -1, -1};
  std::list<int> std_l{1, -1, -23, 4, 1, 5, 6, 4, -1, -1};
  s21_l.unique();
  std_l.unique();
  auto it1 = s21_l.begin();
  auto it2 = std_l.begin();
  while (it1 != s21_l.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
  EXPECT_EQ(s21_l.size(), std_l.size());
}

TEST(ListTest, Splice) {
  s21::list<int> list1{1, 2, 3};
  s21::list<int> list2{4, 5, 6};

  auto it = (list1.begin()++)++;
  list1.splice(it, list2);

  EXPECT_EQ(list1.size(), 6);
  EXPECT_EQ(list2.size(), 0);

  EXPECT_TRUE(list2.empty());
}

TEST(List, Modifier_Sort) {
  s21::list<int> s21_l{1, -1, -14, 4};
  std::list<int> std_l{1, -1, -14, 4};
  s21_l.sort();
  std_l.sort();
  auto it1 = s21_l.begin();
  auto it2 = std_l.begin();
  while (it1 != s21_l.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}

TEST(List, Modifier_Sort2) {
  s21::list<int> s21_list{1, -1, -14, 8, 1, 6, 8, -1, -1};
  std::list<int> std_list{1, -1, -14, 8, 1, 6, 8, -1, -1};
  s21_list.sort();
  std_list.sort();
  auto it1 = s21_list.begin();
  auto it2 = std_list.begin();
  while (it1 != s21_list.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2;
  }
}

TEST(List, Emplace_Test) {
  std::list<int> l_std = {5, 6, 7, 8, 9, 10};
  s21::list<int> l_s21 = {5, 6, 7, 8, 9, 10};

  auto it1 = l_std.emplace(l_std.begin(), 2);
  auto it2 = l_s21.emplace(l_s21.begin(), 2);

  auto it = l_std.begin();
  while (it != l_std.end()) {
    EXPECT_EQ(*it1, *it2);
    ++it1, ++it2, ++it;
  }
}

TEST(List, Emplace_Back_Test) {
  s21::list<int> l_s21 = {};
  l_s21.emplace_back(1);
  l_s21.emplace_back(2);
  l_s21.emplace_back(3);

  EXPECT_DOUBLE_EQ(*(l_s21.begin()++), 1);
  EXPECT_DOUBLE_EQ(*(++l_s21.begin()++), 2);
  EXPECT_DOUBLE_EQ(*(++(++l_s21.begin())), 3);
  EXPECT_EQ(l_s21.size(), 3);
}

TEST(List, Emplace_Front_Test) {
  s21::list<int> l_s21 = {};
  l_s21.emplace_front(3);
  l_s21.emplace_front(2);
  l_s21.emplace_front(1);

  EXPECT_DOUBLE_EQ(*(l_s21.begin()++), 1);
  EXPECT_DOUBLE_EQ(*(++l_s21.begin()++), 2);
  EXPECT_DOUBLE_EQ(*(++(++l_s21.begin())), 3);
  EXPECT_EQ(l_s21.size(), 3);
}
