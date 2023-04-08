#include <gtest/gtest.h>

#include "../s21_containersplus.h"

template <class T>
struct AtTest : public testing::Test {};

using at_types = ::testing::Types<char, int, long double>;

TYPED_TEST_SUITE(AtTest, at_types);

TYPED_TEST(AtTest, empty) {
  s21::array<TypeParam, 0> a{};

  EXPECT_ANY_THROW((void)a.at(0));
  EXPECT_ANY_THROW((void)a.at(1));
  EXPECT_ANY_THROW((void)a.at(100));
}

TEST(ArrayTest, copy_constructor) {
  s21::array<int, 5> arr0{1, 2, 3, 4, 5};
  s21::array<int, 5> arr1{arr0};

  for (std::size_t i = 0; i < arr0.size(); ++i) ASSERT_EQ(arr0[i], arr1[i]);
}

TEST(ArrayTest, move_constructor) {
  s21::array<int, 5> arr0{1, 2, 3, 4, 5};
  s21::array<int, 5> arr1{std::move(arr0)};
  for (std::size_t i = 0; i < arr0.size(); ++i) ASSERT_EQ(arr0[i], arr1[i]);
}

TEST(ArrayTest, copy_assignment) {
  s21::array<int, 5> arr0{1, 2, 3, 4, 5};
  s21::array<int, 5> arr1;
  arr1 = std::move(arr0);
  for (std::size_t i = 0; i < arr0.size(); ++i) ASSERT_EQ(arr0[i], arr1[i]);
}

TEST(ArrayTest, move_assignment) {
  s21::array<int, 5> arr0{1, 2, 3, 4, 5};
  s21::array<int, 5> arr1;
  arr1 = std::move(arr0);
  for (std::size_t i = 0; i < arr0.size(); ++i) ASSERT_EQ(arr0[i], arr1[i]);
}

TEST(ArrayTest, back) {
  const s21::array<int, 5> arr1{1, 2, 3, 4, 5};
  s21::array<int, 5> arr0{1, 2, 3, 4, 5};
  ASSERT_EQ(arr1.back(), 5);
  ASSERT_EQ(arr0.back(), 5);
}

TEST(ArrayTest, data) {
  s21::array<int, 5> arr0{1, 2, 3, 4, 5};
  for (std::size_t i = 0; i < arr0.size(); ++i)
    ASSERT_EQ(*(arr0.data() + i), arr0[i]);
}

TEST(ArrayTest, at) {
  s21::array<int, 5> arr0_{1, 2, 3, 4, 5};
  for (std::size_t i = 0; i < arr0_.size(); ++i)
    ASSERT_EQ(arr0_.at(3), arr0_[3]);
}

TEST(ArrayTest, at_exception) {
  s21::array<int, 5> arr0_{1, 2, 3, 4, 5};
  EXPECT_ANY_THROW(arr0_.at(6));
}

TEST(ArrayTest, size) {
  s21::array<int, 5> arr0_{1, 2, 3, 4, 5};
  ASSERT_EQ(arr0_.size(), 5);
}

TEST(ArrayTest, max_size) {
  s21::array<int, 5> arr0_{1, 2, 3, 4, 5};
  ASSERT_EQ(arr0_.max_size(), 5);
}

TEST(ArrayTest, swap) {
  s21::array<int, 5> arr0_{1, 2, 3, 4, 5};

  s21::array<int, 5> arr1{5, 5, 5, 5, 5};
  std::array<int, 5> arr2{5, 5, 5, 5, 5};
  std::array<int, 5> arr3{1, 2, 3, 4, 5};

  arr2.swap(arr3);
  arr0_.swap(arr1);

  for (s21::array<int, 5>::size_type i = 0; i < arr0_.size(); ++i)
    ASSERT_EQ(arr0_[i], arr3[i]);
}

TEST(ArrayTest, swap_string) {
  s21::array<std::string, 1> arr0{"hello"};
  s21::array<std::string, 1> arr1{"world"};
  std::array<std::string, 1> arr2{"hello"};
  std::array<std::string, 1> arr3{"world"};

  arr0.swap(arr1);
  arr2.swap(arr3);

  for (std::size_t i = 0; i < arr0.size(); ++i) ASSERT_EQ(arr0[i], arr2[i]);
}

TEST(ArrayTest, fill) {
  s21::array<int, 5> arr0_{1, 2, 3, 4, 5};
  arr0_.fill(0);

  for (auto elem : arr0_) ASSERT_EQ(elem, 0);
}

TEST(ArrayTest, empty) {
  s21::array<int, 5> arr0_{1, 2, 3, 4, 5};
  ASSERT_FALSE(arr0_.empty());
}

TEST(Array, test_all) {
  s21::array<std::vector<int>, 10> a;
  std::vector<int> v{1, 2, 3, 4, 5};
  a.fill(v);

  for (auto e : a) ASSERT_EQ(e, v);

  ASSERT_ANY_THROW(a.at(11));
  ASSERT_EQ(a.front(), a[0]);
  ASSERT_EQ(a.front(), *a.data());
  ASSERT_EQ(a.back(), a[9]);
  ASSERT_FALSE(a.empty());
  ASSERT_EQ(a.size(), 10);
  ASSERT_EQ(a.max_size(), 10);

  std::vector<int> w{0, 0, 0, 0, 0};
  s21::array<std::vector<int>, 10> b;
  b.fill(w);
  a.swap(b);

  for (auto e : a) ASSERT_EQ(e, w);

  for (auto e : b) ASSERT_EQ(e, v);
}

TEST(Array, exception) {
  ASSERT_ANY_THROW((s21::array<int, 5>{1, 2, 3, 4, 5, 6, 7}));
  ASSERT_ANY_THROW((s21::array<int, 1>{1, 2, 3, 4, 5, 6, 7}));
  ASSERT_ANY_THROW((s21::array<int, 2>{1, 2, 3, 4, 5, 6, 7}));
  ASSERT_ANY_THROW((s21::array<int, 3>{1, 2, 3, 4, 5, 6, 7}));
  ASSERT_NO_THROW((s21::array<int, 7>{1, 2, 3, 4, 5, 6, 7}));
}