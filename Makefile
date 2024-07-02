FLAGS = -Wall -Werror -Wextra -std=c11
OS := $(shell uname)
FILES = $(wildcard Backend/*.c headers/*.h)
PROJECT = C8_Viewer_1_1

ifeq ($(OS),Linux)
 	OPEN_CMD = open
 	ADD_LIB = -lcheck -lsubunit -lm -lrt -lpthread -D_GNU_SOURSE
endif

ifeq ($(OS),Darwin)
 	OPEN_CMD = open
 	ADD_LIB = -lcheck
endif

all: C8_Viewer_1_1.a test

test: C8_Viewer_1_1.a
	gcc $(FLAGS) test.c C8_Viewer_1_1.a -o test $(ADD_LIB)
	./test

C8_Viewer_1_1.a:
	gcc -c $(FLAGS) $(FILES)
	ar rcs C8_Viewer_1_1.a *.o

gcov_report:
	gcc --coverage $(FILES) test.c -o test $(ADD_LIB)
	./test
	lcov -t "stest" -o s21_test.info -c -d .
	genhtml -o report s21_test.info
	open ./report/index.html

brew:
	cd
	curl -fsSL https://rawgit.com/kube/42homebrew/master/install.sh | zsh

lcov:
	brew install lcov

clean:
	@rm -f test
	@rm -f *.gc*
	@rm -f s21_test.info
	@rm -rf report
	@rm -f *.o *.a
	@rm -rf test.dSYM
	@rm -rf *.tgz
	@rm -rf view/*.o
	@rm -rf headers/*.gch
	@rm -rf *.obj

style:
	@clang-format -style=Google -i $(FILES) test.c

check:
	@clang-format -style=Google -n $(FILES) test.c

install: uninstall
	mkdir build
	@cd C8_Viewer_1_1/ &&qmake && make -j8 && make clean && rm Makefile && mv $(PROJECT).app ../build
	open build

uninstall: clean
	rm -rf build

dvi:
	open Manual.md

dist: clean
	mkdir 3D_Viewer
	mkdir 3D_Viewer/src
	cp -R C8_Viewer_1_1 3D_Viewer/src
	cp -R test.c 3D_Viewer/src
	cp -R screenshot C8_Viewer_1_1/src
	cp -R $(FILES) 3D_Viewer/src
	cp -R Makefile 3D_Viewer/src
	cp -R Manual.md 3D_Viewer/src
	tar cvzf 3D_Viewer.tgz 3D_Viewer/
	rm -rf 3D_Viewer

.PHONY: all test C8_Viewer_1_1.a gcov_report clean dist
