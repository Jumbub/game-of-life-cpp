CC = g++

COMPILER_FLAGS = -Wall -std=c++2a -lpthread
COMPILER_FLAGS_PROFILE = $(COMPILER_FLAGS) -pg
COMPILER_FLAGS_DEBUG = $(COMPILER_FLAGS) -ggdb

LINKER_FLAGS_BENCHMARK = -isystem benchmark/include -Lbenchmark/build/src -lbenchmark
LINKER_FLAGS_GRAPHICS = -lSDL2

OUTPUT = build/out

OBJS = src/util/profile.cpp src/board/next.cpp src/board/generate.cpp
OBJS_GRAPHICS = $(OBJS) src/board/sdl.cpp

main: build
	$(CC) src/entrypoints/main.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	./$(OUTPUT)

debug: build
	$(CC) src/entrypoints/main.cpp -DENABLE_THREADING=0 $(COMPILER_FLAGS_DEBUG) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)

profile: build
	$(CC) src/entrypoints/main.cpp -DENABLE_THREADING=0 $(COMPILER_FLAGS_PROFILE) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)

test: build
	$(CC) src/entrypoints/test.cpp $(COMPILER_FLAGS) -ggdb -o $(OUTPUT) $(OBJS)
	./$(OUTPUT)

benchmark_out: build
	$(CC) src/entrypoints/benchmark.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)

benchmark: build
	make benchmark_out
	./$(OUTPUT) > results/benchmark.txt

build:
	mkdir build
