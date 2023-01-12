CC=g++ -Wall -Werror -Wextra -std=c++17 -g
CHECKFLAGS=-lgtest -lgmock -pthread
GCOVFLAGS = -fprofile-arcs -ftest-coverage
GCOV=--coverage
OS = $(shell uname)

all: test

list.a:
	$(CC) -c list.cpp -o list.o
	ar rcs list.a list.o

test: clean
	$(CC) $(GCOV) -c list.cpp 
	$(CC) -c list.cpp $(CHECKFLAGS)
	$(CC) $(GCOV) -o list list.o list.o $(CHECKFLAGS)
	./list

check:
	cp ../materials/linters/.clang-format ./
	clang-format -i *.h *.cpp
	clang-format -n *.h *.cpp
	rm .clang-format

leaks: test
ifeq ($(OS), Linux)
	CK_FORK=no valgrind --tool=memcheck --leak-check=full ./test
else
	leaks -atExit -- ./test
endif

gcov_report: test
	$(CC) $(CHECKFLAGS) $(GCOVFLAGS) s21_matrix_oop.cpp s21_test.cpp $(CHECKFLAGS) -o gcov_rep.out
	./gcov_rep.out
	lcov -t "gcov_rep" --output-file=coverag.info --capture --directory .
	genhtml -o ./report coverag.info
	open ./report/index.html


clean:
	rm -f *.gcno *.gcda *.info *.out *.gch
	rm -rf ./report
	rm -rf ./*.o ./*.a
	rm -rf test
	rm -rf .clang-format
