#include "array_tests.h"
#include "list_tests.h"
#include "map_tests.h"
#include "multiset_tests.h"
#include "queue_tests.h"
#include "set_tests.h"
#include "stack_test.h"
#include "vector_tests.h"

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}