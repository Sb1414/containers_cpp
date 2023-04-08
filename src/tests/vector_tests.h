#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

#include "../s21_containers.h"

TEST(VectorTest, Constructor_Size) {
  const int size = 5;
  s21::vector<int> vec(size);

  ASSERT_EQ(vec.size(), size);
  ASSERT_EQ(vec.capacity(), size);
  ASSERT_TRUE(vec.data() != nullptr);
}

TEST(VectorTest, InitializerListConstructor_Size) {
  s21::vector<int> vec{1, 2, 3, 4, 5};

  ASSERT_EQ(vec.size(), 5);
  ASSERT_EQ(vec.capacity(), 5);
  ASSERT_TRUE(vec.data() != nullptr);

  for (int i = 0; i < (int)vec.size(); ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

TEST(VectorTest, InitializerListConstructor) {
  std::initializer_list<int> init = {1, 2, 3, 4, 5};
  s21::vector<int> vec(init);

  EXPECT_EQ(vec.size(), init.size());
  EXPECT_EQ(vec.capacity(), init.size());
  int i = 1;
  for (auto it = vec.begin(); it != vec.end(); it++) {
    EXPECT_EQ(*it, i);
    i++;
  }
}

TEST(VectorTest, CopyConstructor) {
  s21::vector<int> vec1{1, 2, 3, 4, 5};
  s21::vector<int> vec2(vec1);

  EXPECT_EQ(vec1.size(), vec2.size());
  EXPECT_EQ(vec1.capacity(), vec2.capacity());
  for (int i = 0; i < (int)vec1.size(); i++) {
    EXPECT_EQ(vec1[i], vec2[i]);
  }
}

TEST(VectorTest, MoveConstructor_Size) {
  s21::vector<int> vec{1, 2, 3, 4, 5};
  s21::vector<int> vec_copy(std::move(vec));

  ASSERT_EQ(vec_copy.size(), 5);
  ASSERT_EQ(vec_copy.capacity(), 5);
  ASSERT_TRUE(vec_copy.data() != nullptr);

  for (int i = 0; i < (int)vec_copy.size(); ++i) {
    ASSERT_EQ(vec_copy[i], i + 1);
  }

  ASSERT_EQ(vec.size(), 0);
  ASSERT_EQ(vec.capacity(), 0);
  ASSERT_TRUE(vec.data() == nullptr);
}

TEST(VectorTest, MoveAssignmentOperator) {
  s21::vector<int> vec1{1, 2, 3, 4, 5};
  int* buffer1 = vec1.data();
  int size1 = vec1.size();
  int capacity1 = vec1.capacity();

  s21::vector<int> vec2;
  vec2 = std::move(vec1);
  int* buffer2 = vec2.data();
  int size2 = vec2.size();
  int capacity2 = vec2.capacity();

  EXPECT_EQ(size1, 5);
  EXPECT_EQ(capacity1, 5);
  EXPECT_EQ(vec1.data(), nullptr);
  EXPECT_EQ(size2, size1);
  EXPECT_EQ(capacity2, capacity1);
  EXPECT_EQ(buffer2, buffer1);
  for (int i = 0; i < size2; i++) {
    EXPECT_EQ(vec2[i], i + 1);
  }
}

TEST(VectorTest, MoveAssignmentOperator_EmptyVector) {
  s21::vector<int> vec;
  s21::vector<int> vec_copy{1, 2, 3, 4, 5};
  vec = std::move(vec_copy);

  ASSERT_EQ(vec.size(), 5);
  ASSERT_EQ(vec.capacity(), 5);
  ASSERT_TRUE(vec.data() != nullptr);

  for (int i = 0; i < (int)vec.size(); ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }

  ASSERT_EQ(vec_copy.size(), 0);
  ASSERT_EQ(vec_copy.capacity(), 0);
  ASSERT_TRUE(vec_copy.data() == nullptr);
}

TEST(VectorTest, CopyAssignmentOperator_Size) {
  s21::vector<int> vec{1, 2, 3, 4, 5};
  s21::vector<int> vec_copy = vec;

  ASSERT_EQ(vec_copy.size(), 5);
  ASSERT_EQ(vec_copy.capacity(), 5);
  ASSERT_TRUE(vec_copy.data() != nullptr);

  for (int i = 0; i < (int)vec_copy.size(); ++i) {
    ASSERT_EQ(vec_copy[i], i + 1);
  }

  ASSERT_EQ(vec.size(), 5);
  ASSERT_EQ(vec.capacity(), 5);
  ASSERT_TRUE(vec.data() != nullptr);

  for (int i = 0; i < (int)vec.size(); ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

TEST(VectorTest, CopyAssignmentOperator_SelfAssignment) {
  s21::vector<int> vec{1, 2, 3, 4, 5};
  ASSERT_EQ(vec.size(), 5);
  ASSERT_EQ(vec.capacity(), 5);
  ASSERT_TRUE(vec.data() != nullptr);

  for (int i = 0; i < (int)vec.size(); ++i) {
    ASSERT_EQ(vec[i], i + 1);
  }
}

TEST(VectorTest, At_Throw) {
  s21::vector<int> vec{1, 2, 3};
  ASSERT_THROW(vec.at(3), std::out_of_range);
}

TEST(VectorTest, At_ValidIndex) {
  s21::vector<int> vec{1, 2, 3};
  ASSERT_EQ(vec.at(0), 1);
  ASSERT_EQ(vec.at(1), 2);
  ASSERT_EQ(vec.at(2), 3);
}

TEST(VectorTest, At_InvalidIndexAndEmptyVector) {
  s21::vector<int> vec{1, 2, 3};
  s21::vector<int> vec2;
  ASSERT_THROW(vec.at(3), std::out_of_range);
  ASSERT_THROW(vec2.at(0), std::out_of_range);
}

TEST(VectorTest, Front_NonEmptyVector) {
  s21::vector<int> vec{1, 2, 3};
  ASSERT_EQ(vec.front(), 1);
}

TEST(VectorTest, Front_EmptyVector) {
  s21::vector<int> vec;
  ASSERT_THROW(vec.front(), std::out_of_range);
}

TEST(VectorTest, Front_ConstEmptyVector) {
  const s21::vector<int> vec{1, 2, 3};
  const s21::vector<int> vec2;
  ASSERT_EQ(vec.front(), 1);
  ASSERT_THROW(vec2.front(), std::out_of_range);
}

TEST(VectorTest, Back_EmptyVector) {
  s21::vector<int> vec{1, 2, 3};
  s21::vector<int> vec2;
  ASSERT_EQ(vec.back(), 3);
  ASSERT_THROW(vec2.back(), std::out_of_range);
}

TEST(VectorTest, Back_ConstEmptyVector) {
  const s21::vector<int> vec{1, 2, 3};
  const s21::vector<int> vec2;
  ASSERT_EQ(vec.back(), 3);
  ASSERT_THROW(vec2.back(), std::out_of_range);
}

TEST(VectorTest, Empty_EmptyVector) {
  s21::vector<int> vec;
  s21::vector<int> vec2{1, 2, 3};
  ASSERT_TRUE(vec.empty());
  ASSERT_FALSE(vec2.empty());
}

TEST(VectorTest, Empty_ConstNonEmptyVector) {
  const s21::vector<int> vec{1, 2, 3};
  const s21::vector<int> vec2;
  ASSERT_FALSE(vec.empty());
  ASSERT_TRUE(vec2.empty());
}

TEST(VectorTest, Empty_ComparisonWithStdVector) {
  s21::vector<int> vec;
  std::vector<int> stdVec;
  ASSERT_EQ(vec.empty(), stdVec.empty());

  vec.push_back(1);
  stdVec.push_back(1);
  ASSERT_EQ(vec.empty(), stdVec.empty());

  vec.push_back(2);
  stdVec.push_back(2);
  ASSERT_EQ(vec.empty(), stdVec.empty());

  vec.pop_back();
  stdVec.pop_back();
  ASSERT_EQ(vec.empty(), stdVec.empty());
}

TEST(VectorTest, Clear) {
  std::vector<int> a;

  EXPECT_DOUBLE_EQ(a.size(), 0);
  EXPECT_DOUBLE_EQ(a.capacity(), 0);

  EXPECT_NO_THROW(a.clear());
  EXPECT_DOUBLE_EQ(a.size(), 0);
  EXPECT_DOUBLE_EQ(a.capacity(), 0);
}

TEST(VectorTest, Reserve) {
  s21::vector<int> a{1, 2, 3};
  size_t old_cap = a.capacity();
  a.reserve(old_cap);
  assert(a.capacity() == old_cap);
  s21::vector<int> b{1, 2, 3};
  b.reserve(10);
  assert(b.capacity() == 10);
}

TEST(VectorTest, Reserve_ThrowsLengthError) {
  s21::vector<int> vec{1, 2, 3};
  ASSERT_THROW(vec.reserve(vec.max_size() + 1), std::length_error);
}

TEST(VectorTest, Reserve_IncreasesCapacity) {
  s21::vector<int> vec{1, 2, 3};
  vec.reserve(10);
  ASSERT_GE(vec.capacity(), 10);
}

TEST(VectorTest, Insert_ComparisonWithStdVector) {
  s21::vector<int> s21vec{1, 2, 3};
  std::vector<int> stdVec = {1, 2, 3};

  auto it = s21vec.insert(s21vec.begin() + 1, 4);
  auto stdIt = stdVec.insert(stdVec.begin() + 1, 4);

  ASSERT_EQ(*it, *stdIt);
  ASSERT_EQ(s21vec.size(), stdVec.size());
  ASSERT_EQ(s21vec.capacity(), stdVec.capacity());
  ASSERT_TRUE(std::equal(s21vec.begin(), s21vec.end(), stdVec.begin()));

  it = s21vec.insert(s21vec.begin(), 0);
  stdIt = stdVec.insert(stdVec.begin(), 0);

  ASSERT_EQ(*it, *stdIt);
  ASSERT_EQ(s21vec.size(), stdVec.size());
  ASSERT_EQ(s21vec.capacity(), stdVec.capacity());
  ASSERT_TRUE(std::equal(s21vec.begin(), s21vec.end(), stdVec.begin()));

  it = s21vec.insert(s21vec.end(), 5);
  stdIt = stdVec.insert(stdVec.end(), 5);

  ASSERT_EQ(*it, *stdIt);
  ASSERT_EQ(s21vec.size(), stdVec.size());
  ASSERT_EQ(s21vec.capacity(), stdVec.capacity());
  ASSERT_TRUE(std::equal(s21vec.begin(), s21vec.end(), stdVec.begin()));
}

TEST(VectorTest, Insert_ThrowsOutOfRange) {
  s21::vector<int> vec{1, 2, 3};
  ASSERT_THROW(vec.insert(vec.begin() + 5, 4), std::out_of_range);
}

TEST(VectorTest, Insert_IncreasesSize) {
  s21::vector<int> vec{1, 2, 3};
  vec.insert(vec.begin() + 1, 4);
  ASSERT_EQ(vec.size(), 4);
}

TEST(VectorTest, Insert_AddsValue) {
  s21::vector<int> vec{1, 2, 3};
  vec.insert(vec.begin() + 1, 4);
  ASSERT_EQ(vec[1], 4);
}

TEST(VectorTest, TestInsertException) {
  s21::vector<int> vec{1, 2, 3};
  EXPECT_THROW(vec.insert(vec.begin() + 5, 4), std::out_of_range);
}

TEST(VectorTest, Erase_ThrowsOutOfRange) {
  s21::vector<int> vec{1, 2, 3};
  ASSERT_THROW(vec.erase(vec.begin() + 5), std::out_of_range);
}

TEST(VectorTest, Erase_DecreasesSize) {
  s21::vector<int> vec{1, 2, 3};
  vec.erase(vec.begin() + 1);
  ASSERT_EQ(vec.size(), 2);
}

TEST(VectorTest, Erase_RemovesValue) {
  s21::vector<int> vec{1, 2, 3};
  vec.erase(vec.begin() + 1);
  ASSERT_EQ(vec[1], 3);
}

TEST(VectorTest, Erase_Vector) {
  s21::vector<int> s21_vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto it_s21 = s21_vec.erase(s21_vec.begin() + 3);
  std::vector<int> std_vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto it_std = std_vec.erase(std_vec.begin() + 3);

  ASSERT_EQ(*it_s21, *it_std);

  for (auto i = std_vec.size() - 1; i < std_vec.size(); --i)
    ASSERT_EQ(s21_vec[i], std_vec[i]);

  ASSERT_EQ(s21_vec.size(), std_vec.size());
  ASSERT_EQ(s21_vec.capacity(), std_vec.capacity());
}

TEST(VectorTest, Data) {
  s21::vector<int> vec{1};
  ASSERT_EQ(vec.data(), &vec[0]);
  ASSERT_EQ(*vec.data(), vec[0]);
  ASSERT_EQ(vec.data(), vec.begin());
}

TEST(VectorTest, DataConst) {
  const s21::vector<std::string> s21_vec{
      "evetteis",
      "hsiuayes",
      "bryceaet",
  };
  const std::vector<std::string> std_vec{
      "evetteis",
      "hsiuayes",
      "bryceaet",
  };
  ASSERT_EQ(s21_vec.size(), std_vec.size());
  ASSERT_EQ(s21_vec.capacity(), std_vec.capacity());
  ASSERT_EQ(*s21_vec.data(), *std_vec.data());
}

TEST(VectorTest, OperatorConst) {
  const s21::vector<std::string> s21_vec{
      "evetteis",
      "hsiuayes",
      "bryceaet",
  };
  const std::vector<std::string> std_vec{
      "evetteis",
      "hsiuayes",
      "bryceaet",
  };
  ASSERT_EQ(*s21_vec.data(), *std_vec.data());
  for (size_t i = 0; i < s21_vec.size(); ++i) ASSERT_EQ(s21_vec[i], std_vec[i]);
}

TEST(VectorTest, Swap) {
  s21::vector<int> vec1{1, 2, 3, 4, 5};
  s21::vector<int> vec2{5, 4, 3, 2, 1};
  auto a = vec2;
  auto b = vec1;

  vec1.swap(vec2);

  for (auto i = a.size() - 1; i < a.size(); --i) ASSERT_EQ(a[i], vec1[i]);
  ASSERT_EQ(a.size(), vec1.size());
  ASSERT_EQ(a.capacity(), vec1.capacity());

  for (auto i = b.size() - 1; i < b.size(); --i) ASSERT_EQ(b[i], vec2[i]);
  ASSERT_EQ(b.size(), vec2.size());
  ASSERT_EQ(b.capacity(), vec2.capacity());
}

TEST(VectorTest, Emplace_InsertsElementAtSpecifiedPosition) {
  s21::vector<int> vec{1, 2, 3};
  auto it = vec.emplace(vec.begin() + 1, 4);

  ASSERT_EQ(*it, 4);
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 4);
  ASSERT_EQ(vec[2], 2);
  ASSERT_EQ(vec[3], 3);
}

TEST(VectorTest, Emplace_IncreasesSizeAndCapacity) {
  s21::vector<int> vec{1, 2, 3};
  vec.emplace_back(4);

  ASSERT_EQ(vec.size(), 4);
  ASSERT_GE(vec.capacity(), vec.size());
}

TEST(VectorTest, EmplaceBack_InsertsElementAtEndOfVector) {
  s21::vector<int> vec{1, 2, 3};
  auto it = vec.emplace_back(4, 5);

  ASSERT_EQ(*it, 5);
  ASSERT_EQ(vec[0], 1);
  ASSERT_EQ(vec[1], 2);
  ASSERT_EQ(vec[2], 3);
  ASSERT_EQ(vec[3], 4);
}