CC=gcc
CFLAGS= -std=c11 -pedantic 

SOURCES=*.c
OBJECTS=*.o

all: s21_viewer_3d.a test install

s21_viewer_3d.a:
	$(CC) $(CFLAGS) -g -c $(SOURCES)
	ar rc s21_viewer_3d.a $(OBJECTS)
	ranlib s21_viewer_3d.a
	rm -f *.o

test: s21_viewer_3d.a
	$(CC) $(CFLAGS) -g -c s21_viewer_test.c
	ar rc s21_viewer_test.a s21_viewer_test.o
	ranlib s21_viewer_test.a
	$(CC) $(CFLAGS) -g --coverage -o test_cases s21_viewer_3d.a s21_viewer_test.a $(SOURCES) -lcheck -lm -pthread
	./test_cases

clean:
	rm -rf *.a && rm -rf *.o *.exe test_cases
	rm -rf *.info && rm -rf *.gcda && rm -rf *.gcno &&  rm -rf *.gcov *.html *.css *.cfg
	rm -rf report/ && rm -rf *.out
	rm -rf .clang-format manual

gcov_report: test
	gcovr -r . --html --html-details -o report.html
	./test_cases
	rm *.gcda *.gcno *.a *.o
	open report.html

install:
	cd CPP_FILES && cmake .
	cd CPP_FILES && make
	cd CPP_FILES/view.app/Contents/MacOS && ./view

uninstall:
	cd CPP_FILES && make clean
	rm -rf CPP_FILES/CMakeCache.txt CPP_FILES/CPP_FILES.app
	rm -rf CPP_FILES/Makefile CPP_FILES/CMakeCache.txt CPP_FILES/cmake_install.cmake CPP_FILES/CMakeFiles
	rm -rf CPP_FILES/view.app


dist:
	cd ../ && tar -czf 3DViewer.tgz src

dvi:
	makeinfo manual.texi --html
	open ./manual/index.html

dvi_clean:
	rm -rf manual

check:
	cp ../materials/linters/.clang-format .clang-format
	clang-format -n CPP_FILES/*.c
	clang-format -n CPP_FILES/*.h
	clang-format -n CPP_FILES/*.cpp
	clang-format -n CPP_FILES/*.ui
	clang-format -n *.c
	clang-format -n *.h
	cppcheck CPP_FILES/*.c
	cppcheck --language=c++ CPP_FILES/*.cpp
	cppcheck --language=c++ CPP_FILES/*.h
	cppcheck *.c
	cppcheck *.h
