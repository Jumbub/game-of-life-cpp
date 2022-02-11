# Build & run executables

main: main_exe
	./build/main

test: test_exe
	./build/test

benchmark: benchmark_exe
	./build/bench

benchmark_all: all_exe
	echo "\nOfast" >> benchmark_all.txt
	./build/bench-Ofast >> benchmark_all.txt
	echo "\nO3" >> benchmark_all.txt
	./build/bench-O3 >> benchmark_all.txt
	echo "\nO2" >> benchmark_all.txt
	./build/bench-O2 >> benchmark_all.txt
	echo "\nO1" >> benchmark_all.txt
	./build/bench-O1 >> benchmark_all.txt
	echo "\nO0" >> benchmark_all.txt
	./build/bench-O0 >> benchmark_all.txt
	echo "\nOs" >> benchmark_all.txt
	./build/bench-Os >> benchmark_all.txt

# Build executables

main_exe: build/build.ninja
	cmake --build build --target main

debug_exe: build/build.ninja
	cmake --build build --target debug

test_exe: build/build.ninja
	cmake --build build --target test

benchmark_exe: build/build.ninja
	cmake --build build --target bench

all_exe: build/build.ninja
	cmake --build build

# Other

build/build.ninja:
	mkdir -p build && cd build && cmake -GNinja ../ || rm -rf build

test_github:
	g++ src/entrypoints/test.cpp -Wall -Wextra -Werror -Wpedantic -Wsign-conversion -std=c++2a -lpthread -O3 -o test
	./test
