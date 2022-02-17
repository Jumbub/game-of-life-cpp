# Build & run executables

main: main_exe
	./build/src/main

test: test_exe
	./build/src/test

benchmark: benchmark_exe
	./build/src/bench

debug: debug_exe
	gdb ./build/src/debug

benchmark_all: all_exe
	echo "Ofast" >> benchmark_all.txt
	./build/src/bench-Ofast >> benchmark_all.txt
	echo "O3" >> benchmark_all.txt
	./build/src/bench-O3 >> benchmark_all.txt
	echo "O2" >> benchmark_all.txt
	./build/src/bench-O2 >> benchmark_all.txt
	echo "O1" >> benchmark_all.txt
	./build/src/bench-O1 >> benchmark_all.txt
	echo "O0" >> benchmark_all.txt
	./buil/src/bench-O0 >> benchmark_all.txt
	echo "Os" >> benchmark_all.txt
	./build/src/bench-Os >> benchmark_all.txt

# Build executables & IDE compile commands

main_exe: build/build.ninja
	cmake --build build --target main
	make compile_commands

debug_exe: build/build.ninja
	cmake --build build --target debug
	make compile_commands

test_exe: build/build.ninja
	cmake --build build --target test
	make compile_commands

benchmark_exe: build/build.ninja
	cmake --build build --target bench
	make compile_commands

all_exe: build/build.ninja
	cmake --build build
	make compile_commands

# Other

compile_commands:
	ninja -C build -t compdb > compile_commands.json

build/build.ninja:
	mkdir -p build && cd build && cmake -GNinja ../ || rm -rf build

test_github:
	g++ src/entrypoints/test.cpp -Wall -Wextra -Werror -Wpedantic -Wsign-conversion -std=c++2a -lpthread -O3 -o test
	./test
