CC = g++

COMPILER_FLAGS = -Wall -std=c++2a -lpthread
COMPILER_FLAGS_BENCHMARK = $(COMPILER_FLAGS) -isystem benchmark/include -Lbenchmark/build/src -lbenchmark
COMPILER_FLAGS_PROFILE = $(COMPILER_FLAGS) -pg
COMPILER_FLAGS_DEBUG = $(COMPILER_FLAGS) -ggdb

LINKER_FLAGS_GRAPHICS = -lSDL2

OUTPUT = build/out
OUTPUT_BENCHMARK = build/out
OUTPUT_DEBUG = build/out
OUTPUT_TEST = build/out

OBJS = src/util/profile.cpp src/board/next.cpp src/board/generate.cpp
OBJS_GRAPHICS = $(OBJS) src/board/sdl.cpp

interactive: build
	$(CC) src/entrypoints/interactive.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	./$(OUTPUT)

debug: build
	$(CC) src/entrypoints/interactive.cpp $(COMPILER_FLAGS_DEBUG) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT_DEBUG) $(OBJS_GRAPHICS)

test: build
	$(CC) src/entrypoints/test.cpp $(COMPILER_FLAGS) -ggdb -o $(OUTPUT_TEST) $(OBJS)
	./$(OUTPUT_TEST)

benchmark: build
	$(CC) src/entrypoints/benchmark.cpp $(COMPILER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT_BENCHMARK) $(OBJS_GRAPHICS)
	./$(OUTPUT_BENCHMARK) > results/benchmark.txt

build:
	mkdir build
