CFLAGS = -Wall -Werror -Wextra -std=c11
GCOV = -fprofile-arcs -ftest-coverage
CC = gcc
LIBS = -lcheck
RM = rm -rf
SRCS = parser.c affins.c
TARGET = 3d_viewer
OBJS = $(SRCS:.c=.o)
OS := $(shell uname -s)

ifeq ($(OS), Linux)
CFLAGS += -D OS_LINUX
ADD_LIB = -lcheck -lm -lrt -lpthread
TEST_FLAGS =-lpthread -lcheck -pthread -lrt -lm
endif
ifeq ($(OS), Darwin)
CFLAGS += -D OS_MAC
ADD_LIB =
TEST_FLAGS =-lcheck
endif

all: viewer

viewer: viewer.a
	$(CC) viewer_tests.c -lcheck -L. viewer.a -o viewer

viewer.a:
	$(CC) -c $(CFLAGS) $(SRCS)
	ar rcs viewer.a $(OBJS)
	ranlib viewer.a

install:
	qmake -o Executables/Makefile $(TARGET)/$(TARGET).pro
	cd Executables && make

start: install
	cd Executables/$(TARGET).app/Contents/MacOS && ./$(TARGET)

uninstall: clean
	rm -rf Executables

dist: install 
	cd .. && tar -czf $(TARGET).tgz src

dvi:
	open ../README_RUS.md

test: viewer.a
	$(CC) viewer_tests.c -lcheck -L. viewer.a -o viewer_tests
	./viewer_tests

gcov_report:
	$(CC) $(GCOV) viewer_tests.c $(SRCS) -o viewer_tests -lcheck
	./viewer_tests
	lcov -t "test" -o test.info -c -d ./
	genhtml test.info -o report
	open report/index.html

clean:
	$(RM) *.a *.o *.info *.gcno *.gcda *.gcov viewer_tests report a.out viewer .clang-format

valgrind_check: test
	CK_FORK=no valgrind --vgdb=no --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./viewer_tests

leak: test
	CK_FORK=no leaks -atExit -- ./viewer_tests

style:
	cp ../materials/linters/.clang-format ./
	clang-format -n *.c *.h ./3d_viewer/*.cpp ./3d_viewer/*.h
	rm .clang-format

style_change:
	clang-format -style=Google -i *.c *.h ./3d_viewer/*.cpp ./3d_viewer/*.h

install_brew:
	cd ~
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh
	brew install lcov

rebuild:
	$(MAKE) clean
	$(MAKE) all

.PHONY: all install clean test viewer_tests.a gcov_report
