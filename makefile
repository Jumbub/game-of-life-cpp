CC = g++

COMPILER_FLAGS = -Wall -std=c++2a
COMPILER_FLAGS_BENCHMARK = $(COMPILER_FLAGS) -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread
COMPILER_FLAGS_BENCHMARK_PROFILE = $(COMPILER_FLAGS_BENCHMARK)

LINKER_FLAGS_GRAPHICS = -lSDL2

OUTPUT = build/interactive
OUTPUT_BENCHMARK = build/benchmark
OUTPUT_BENCHMARK_PROFILE = build/benchmark_profile
OUTPUT_TEST = build/test

OBJS = src/util/profile.cpp src/board/next.cpp src/board/generate.cpp
OBJS_GRAPHICS = $(OBJS) src/board/sdl.cpp

build:
	mkdir build

interactive: build
	$(CC) src/entrypoints/interactive.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	./$(OUTPUT)

test: build
	$(CC) src/entrypoints/test.cpp $(COMPILER_FLAGS) -ggdb -o $(OUTPUT_TEST) $(OBJS)
	./$(OUTPUT_TEST)

benchmark: build
	$(CC) src/entrypoints/benchmark.cpp $(COMPILER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT_BENCHMARK) $(OBJS_GRAPHICS)
	./$(OUTPUT_BENCHMARK) > results/benchmark.txt

profile: build
	$(CC) src/entrypoints/interactive.cpp $(COMPILER_FLAGS_BENCHMARK_PROFILE) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT_BENCHMARK_PROFILE) $(OBJS_GRAPHICS)
