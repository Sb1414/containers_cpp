#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <queue>
#include <vector>

#include "../s21_containers.h"

TEST(queueTest, default_constructor) {
  s21::queue<int> q{};
  ASSERT_TRUE(q.empty());
  ASSERT_EQ(q.size(), 0);
}

TEST(queueTest, initializer_list_constructor) {
  s21::queue<char> q{'e', 'v', 'e', 't', 't', 'e', 'i', 's'};
  ASSERT_FALSE(q.empty());
  ASSERT_EQ(q.front(), 'e');
  ASSERT_EQ(q.back(), 's');
  ASSERT_EQ(q.size(), 8);
  for (auto i = 0; i < 8; i++) {
    q.pop();
  }
  ASSERT_TRUE(q.empty());
}

TEST(queueTest, copy_constructor) {
  s21::queue<int> q{1, 2, 3, 4, 5};
  s21::queue<int> copy(q);
  copy.push(14);
  q.push(14);
  while (!copy.empty()) {
    ASSERT_EQ(copy.front(), q.front());
    copy.pop();
    q.pop();
  }
  ASSERT_TRUE(copy.empty());
  ASSERT_TRUE(q.empty());
}

TEST(queueTest, move_constructor) {
  s21::queue<char> q{'e', 'v', 'e', 't', 't', 'e', 'i', 's'};
  s21::queue<char> moved(std::move(q));
  ASSERT_EQ(moved.size(), 8);
}

TEST(queueTest, swap) {
  s21::queue<int> q1{1, 2, 3, 4, 5};
  s21::queue<int> q2{};
  ASSERT_EQ(q2.size(), 0);
  ASSERT_EQ(q1.size(), 5);
  ASSERT_FALSE(q1.empty());
  ASSERT_TRUE(q2.empty());
  q1.swap(q2);
  ASSERT_FALSE(q2.empty());
  ASSERT_TRUE(q1.empty());
  ASSERT_EQ(q1.size(), 0);
  ASSERT_EQ(q2.size(), 5);
}

TEST(queueTest, emplace_back) {
  s21::queue<std::vector<int>> s;
  std::queue<std::vector<int>> ss;
  for (s21::queue<std::vector<int>>::size_type i = 0; i < 10; ++i) {
    s.emplace_back((std::vector<int>{1, 2, 3, 4, 5}),
                   (std::vector<int>{1, 2, 3, 4, 5}),
                   (std::vector<int>{1, 2, 3, 4, 5}));
    ss.push(std::vector<int>{1, 2, 3, 4, 5});
    ss.push(std::vector<int>{1, 2, 3, 4, 5});
    ss.push(std::vector<int>{1, 2, 3, 4, 5});
  }

  ASSERT_EQ(s.size(), ss.size());
}

TEST(queueTest, consts) {
  s21::queue<int> x;
  std::queue<int> y;
  int val = 14;
  x.push(val);
  y.push(val);
  x.push(val);
  y.push(val);

  const s21::queue<int> cx = x;
  const std::queue<int> cy = y;

  ASSERT_EQ(cx.front(), cy.front());
  ASSERT_EQ(cx.back(), cy.back());
}

TEST(queueTest, back) {
  s21::queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  q.push(4);
  q.push(5);
  while (!q.empty()) {
    ASSERT_EQ(q.back(), 5);
    q.pop();
  }
  ASSERT_TRUE(q.empty());
}