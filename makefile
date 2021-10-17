main: build
	cmake --build build --target main
	./build/main

benchmark: build
	cmake --build build --target bench
	./build/bench > results/benchmark.txt
	cat results/benchmark.txt

benchmark_short: build
	cmake --build build --target bench_short
	./build/bench

test: build
	cmake --build build --target test
	./build/test

debug_exe: build
	cmake --build build --target debug

profile_exe: build
	cmake --build build --target profile

benchmark_exe: build
	cmake --build build --target benchmark
	cp ./build/benchmark ./build/out
	./build/out

benchmark_full: build
	cmake --build build
	echo Ofast >> results/benchmark_full.txt
	./build/bench-Ofast >> results/benchmark_full.txt
	echo O3 >> results/benchmark_full.txt
	./build/bench-O3 >> results/benchmark_full.txt
	echo O2 >> results/benchmark_full.txt
	./build/bench-O2 >> results/benchmark_full.txt
	echo O1 >> results/benchmark_full.txt
	./build/bench-O1 >> results/benchmark_full.txt
	echo O0 >> results/benchmark_full.txt
	./build/bench-O0 >> results/benchmark_full.txt
	echo Os >> results/benchmark_full.txt
	./build/bench-Os >> results/benchmark_full.txt

build:
	mkdir -p build && cd build && cmake -GNinja ../
