#include <gtest/gtest.h>

#include <stack>
#include <vector>

#include "../s21_containers.h"

TEST(stackTest, default_constructor) {
  s21::stack<int> s1_{};
  ASSERT_TRUE(s1_.empty());
  ASSERT_EQ(s1_.size(), 0);
}

TEST(Test1, initializer_list_constructor) {
  s21::stack<int> a{0, 1, 2, 3, 4, 5};
  ASSERT_EQ(a.top(), 5);
  ASSERT_FALSE(a.empty());
  ASSERT_EQ(a.size(), 6);
  a.pop();
  ASSERT_EQ(a.size(), 5);
  a.pop();
  ASSERT_EQ(a.size(), 4);
  ASSERT_EQ(a.top(), 3);
  a.pop();
  a.pop();
  a.pop();
  a.pop();
  ASSERT_TRUE(a.empty());
}

TEST(stackTest, copy_constructor) {
  s21::stack<int> a{14, 13, 12, 11, 10};
  s21::stack<int> copy(a);
  ASSERT_EQ(copy.top(), a.top());
  copy.pop();
  a.pop();
  ASSERT_EQ(copy.top(), a.top());
  copy.pop();
  a.pop();
  ASSERT_EQ(copy.top(), a.top());
  copy.pop();
  a.pop();
  ASSERT_EQ(copy.top(), a.top());
  copy.pop();
  a.pop();
  ASSERT_EQ(copy.top(), a.top());
  copy.pop();
  a.pop();

  ASSERT_TRUE(copy.empty());
  ASSERT_TRUE(a.empty());

  int x = 18;
  copy.push(x);
  a.push(x);
  copy.push(69);
  a.push(69);
  ASSERT_EQ(copy.top(), a.top());
}

TEST(stackTest, move_constructor) {
  s21::stack<int> a{14, 13, 12, 11, 10, 9};
  s21::stack<int> moved(std::move(a));
  ASSERT_EQ(moved.size(), 6);
  ASSERT_EQ(moved.top(), 9);
}

TEST(stackTest, copy_assignment) {
  s21::stack<int> copy;
  s21::stack<int> orig{};
  ASSERT_EQ(copy.top(), orig.top());
  copy.pop();
  orig.pop();
  ASSERT_EQ(copy.top(), orig.top());
  copy.pop();
  orig.pop();
  ASSERT_EQ(copy.top(), orig.top());
  copy.pop();
  orig.pop();
  ASSERT_EQ(copy.top(), orig.top());
  copy.pop();
  orig.pop();
  ASSERT_EQ(copy.top(), orig.top());
  copy.pop();
  orig.pop();

  ASSERT_TRUE(copy.empty());
  ASSERT_TRUE(orig.empty());

  copy.push(8);
  orig.push(8);
  ASSERT_EQ(copy.top(), orig.top());
}

TEST(stackTest, move_assignment) {
  s21::stack<int> moved;
  s21::stack<int> a{14, 13, 12, 11, 10, 9};
  moved = std::move(a);
  ASSERT_EQ(moved.size(), 6);
  ASSERT_EQ(moved.top(), 9);
}

TEST(stackTest, swap) {
  s21::stack<int> a{14, 13, 12, 11, 10, 9};
  s21::stack<int> b{};
  ASSERT_EQ(b.size(), 0);
  ASSERT_EQ(a.size(), 6);
  ASSERT_FALSE(a.empty());
  ASSERT_TRUE(b.empty());
  a.swap(b);
  ASSERT_FALSE(b.empty());
  ASSERT_TRUE(a.empty());
  ASSERT_EQ(a.size(), 0);
  ASSERT_EQ(b.size(), 6);
}

TEST(stack, consts) {
  s21::stack<int> x;
  std::stack<int> y;
  int val = 11;
  x.push(val);
  y.push(val);
  x.push(val);
  x.push(val);
  y.push(val);
  y.push(val);
}

TEST(StackTest, swap_test) {
  std::stack<int> a({1, 2, 3, 4, 5});
  std::stack<int> b(a);
  ASSERT_EQ(a.size(), 5);

  for (int i = 0; i < 5; ++i) {
    ASSERT_EQ(a.top(), b.top());
    a.pop();
    b.pop();
  }
}

TEST(stackTest, push_pop_top_empty_size) {
  std::stack<int> a{};
  std::stack<int> b;
  ASSERT_EQ(a.size(), 0);
  ASSERT_TRUE(a.empty());
  for (int i = 0; i < 5; i++) {
    a.push(i);
    b.push(i);
    ASSERT_EQ(a.top(), b.top());
    ASSERT_EQ(a.size(), b.size());
    ASSERT_FALSE(a.empty());
  }

  for (int i = 0; i < 4; i++) {
    a.pop();
    b.pop();
    ASSERT_EQ(a.top(), b.top());
    ASSERT_EQ(a.size(), b.size());
    ASSERT_FALSE(a.empty());
  }
  a.pop();
  b.pop();
  ASSERT_EQ(a.size(), b.size());
  ASSERT_TRUE(a.empty());
}

TEST(stackTest, top_pop_push_size_empty) {
  std::stack<int> a;
  std::stack<int> b{};
  ASSERT_EQ(b.size(), 0);
  ASSERT_TRUE(b.empty());

  for (int i = 0; i < 5; i++) {
    b.push(i);
    a.push(i);
    ASSERT_EQ(b.top(), a.top());
    ASSERT_EQ(b.size(), a.size());
    ASSERT_FALSE(b.empty());
  }
}

TEST(Stack, copy_constructor) {
  s21::stack<int> s1;
  s1.push(1);
  s1.push(2);
  s1.push(3);
  s1.push(4);
  s21::stack<int> s0(s1);
  for (int i = 0; i < 3; i++) {
    s1.pop();
    s0.pop();
    ASSERT_EQ(s1.size(), s0.size());
    ASSERT_EQ(s1.top(), s0.top());
    ASSERT_FALSE(s1.empty());
    ASSERT_FALSE(s0.empty());
  }
  s1.pop();
  s0.pop();
  ASSERT_EQ(s1.size(), s0.size());
  ASSERT_TRUE(s1.empty());
  ASSERT_TRUE(s0.empty());
}

TEST(Stack, move_constructor) {
  s21::stack<int> s1;
  s1.push(1);
  s1.push(2);
  s1.push(3);
  s1.push(4);
  s1.push(5);

  s21::stack<int> s0(std::move(s1));
  ASSERT_EQ(s1.size(), 0);
  ASSERT_EQ(s0.size(), 5);
  ASSERT_TRUE(s1.empty());
  ASSERT_FALSE(s0.empty());

  ASSERT_EQ(s0.top(), 5);
  s0.pop();
  ASSERT_EQ(s0.top(), 4);
  s0.pop();
  ASSERT_EQ(s0.top(), 3);
  s0.pop();
  ASSERT_EQ(s0.top(), 2);
  s0.pop();
  ASSERT_EQ(s0.top(), 1);
  s0.pop();
  ASSERT_TRUE(s0.empty());
}

TEST(Stack, constructor) {
  s21::stack<int> s0;
  ASSERT_EQ(s0.size(), 0);
  ASSERT_TRUE(s0.empty());
}

TEST(Stack, copy_assignment) {
  s21::stack<int> s1;
  s1.push(1);
  s1.push(2);
  s1.push(3);
  s1.push(4);
  s21::stack<int> s0;
  s0 = s1;
  for (int i = 0; i < 3; i++) {
    s1.pop();
    s0.pop();
    ASSERT_EQ(s1.size(), s0.size());
    ASSERT_EQ(s1.top(), s0.top());
    ASSERT_FALSE(s1.empty());
    ASSERT_FALSE(s0.empty());
  }
  s1.pop();
  s0.pop();
  ASSERT_EQ(s1.size(), s0.size());
  ASSERT_TRUE(s1.empty());
  ASSERT_TRUE(s0.empty());
}

TEST(Stack, move_assignment) {
  s21::stack<int> s1;
  s1.push(1);
  s1.push(2);
  s1.push(3);
  s1.push(4);
  s1.push(5);

  s21::stack<int> s0;
  s0 = std::move(s1);
  ASSERT_EQ(s1.size(), 0);
  ASSERT_EQ(s0.size(), 5);
  ASSERT_TRUE(s1.empty());
  ASSERT_FALSE(s0.empty());

  ASSERT_EQ(s0.top(), 5);
  s0.pop();
  ASSERT_EQ(s0.top(), 4);
  s0.pop();
  ASSERT_EQ(s0.top(), 3);
  s0.pop();
  ASSERT_EQ(s0.top(), 2);
  s0.pop();
  ASSERT_EQ(s0.top(), 1);
  s0.pop();
  ASSERT_TRUE(s0.empty());
}

TEST(Stack, initializer_list) {
  s21::stack<int> s0{1, 2, 3, 4, 5};
  ASSERT_EQ(s0.size(), 5);
  ASSERT_FALSE(s0.empty());

  ASSERT_EQ(s0.top(), 5);
  s0.pop();
  ASSERT_EQ(s0.top(), 4);
  s0.pop();
  ASSERT_EQ(s0.top(), 3);
  s0.pop();
  ASSERT_EQ(s0.top(), 2);
  s0.pop();
  ASSERT_EQ(s0.top(), 1);
  s0.pop();
  ASSERT_TRUE(s0.empty());
}

TEST(Stack, push_pop) {
  s21::stack<int> s0;
  s0.push(5);
  s0.push(4);
  s0.push(3);
  s0.push(2);
  s0.push(1);
  ASSERT_EQ(s0.size(), 5);
  ASSERT_FALSE(s0.empty());
  for (int i = 1; i <= 5; i++) {
    ASSERT_EQ(s0.top(), i);
    s0.pop();
  }
  ASSERT_TRUE(s0.empty());
}

TEST(Stack, top) {
  s21::stack<int> s0;
  s0.push(1);
  s0.push(2);
  s0.push(3);
  s0.push(4);
  s0.push(5);
  ASSERT_EQ(s0.size(), 5);
  ASSERT_FALSE(s0.empty());
  for (int i = 5; i > 0; i--) {
    ASSERT_EQ(s0.top(), i);
    s0.pop();
  }
  ASSERT_TRUE(s0.empty());
}

TEST(Stack, size) {
  s21::stack<int> s0;
  ASSERT_EQ(s0.size(), 0);
  ASSERT_TRUE(s0.empty());
  s0.push(1);
  ASSERT_EQ(s0.size(), 1);
  ASSERT_FALSE(s0.empty());
}

TEST(stack, emplace_back) {
  s21::stack<std::vector<int>> s;
  std::stack<std::vector<int>> ss;
  for (s21::stack<std::vector<int>>::size_type i = 0; i < 10; ++i) {
    s.emplace_front((std::vector<int>{1, 2, 3, 4, 5}),
                    (std::vector<int>{1, 2, 3, 4, 5}),
                    (std::vector<int>{1, 2, 3, 4, 5}));
    ss.push(std::vector<int>{1, 2, 3, 4, 5});
    ss.push(std::vector<int>{1, 2, 3, 4, 5});
    ss.push(std::vector<int>{1, 2, 3, 4, 5});
  }
  const s21::stack<std::vector<int>> cs = s;
  ASSERT_EQ(s.size(), ss.size());
  for (s21::stack<std::vector<int>>::size_type i = 0; i < s.size(); i++) {
    ASSERT_EQ(s.top(), ss.top());
    s.pop();
    ss.pop();
  }
  ASSERT_EQ(s.empty(), ss.empty());
}