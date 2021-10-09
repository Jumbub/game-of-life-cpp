CC = g++

COMPILER_FLAGS = -Wall -Wextra -Werror -Wpedantic -Wsign-conversion -std=c++2a -lpthread
COMPILER_FLAGS_OPTIMIZED = $(COMPILER_FLAGS) -lpthread -O1
COMPILER_FLAGS_PROFILE = $(COMPILER_FLAGS) -Og -pg
COMPILER_FLAGS_DEBUG = $(COMPILER_FLAGS) -Og -ggdb

LINKER_FLAGS_BENCHMARK = -isystem benchmark/include -Lbenchmark/build/src -lbenchmark
LINKER_FLAGS_GRAPHICS = -lSDL2

OUTPUT = build/out

# OBJS = src/util/profile.cpp src/board/next.cpp src/board/generate.cpp
# OBJS_GRAPHICS = $(OBJS) src/board/sdl.cpp

main: build
	$(CC) src/entrypoints/main.cpp $(COMPILER_FLAGS_OPTIMIZED) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	./$(OUTPUT)

debug: build
	$(CC) src/entrypoints/main.cpp -DENABLE_THREADING=0 $(COMPILER_FLAGS_DEBUG) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)

test: build
	$(CC) src/entrypoints/test.cpp $(COMPILER_FLAGS_OPTIMIZED) -ggdb -o $(OUTPUT) $(OBJS)
	./$(OUTPUT)

benchmark_out: build
	$(CC) src/entrypoints/benchmark.cpp $(COMPILER_FLAGS_OPTIMIZED) $(LINKER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)

benchmark_raw: build
	$(CC) src/entrypoints/benchmark.cpp $(COMPILER_FLAGS_OPTIMIZED) $(LINKER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	./$(OUTPUT)

benchmark: build
	make benchmark_out
	./$(OUTPUT) > results/benchmark.txt

benchmark_full: build
	$(CC) src/entrypoints/benchmark_full.cpp $(COMPILER_FLAGS) -O0 $(LINKER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	echo O0 > results/benchmark_full.txt
	./$(OUTPUT) >> results/benchmark_full.txt
	$(CC) src/entrypoints/benchmark_full.cpp $(COMPILER_FLAGS) -O1 $(LINKER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	echo O1 >> results/benchmark_full.txt
	./$(OUTPUT) >> results/benchmark_full.txt
	$(CC) src/entrypoints/benchmark_full.cpp $(COMPILER_FLAGS) -O2 $(LINKER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	echo O2 >> results/benchmark_full.txt
	./$(OUTPUT) >> results/benchmark_full.txt
	$(CC) src/entrypoints/benchmark_full.cpp $(COMPILER_FLAGS) -O3 $(LINKER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	echo O3 >> results/benchmark_full.txt
	./$(OUTPUT) >> results/benchmark_full.txt
	$(CC) src/entrypoints/benchmark_full.cpp $(COMPILER_FLAGS) -Ofast $(LINKER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	echo Ofast >> results/benchmark_full.txt
	./$(OUTPUT) >> results/benchmark_full.txt
	$(CC) src/entrypoints/benchmark_full.cpp $(COMPILER_FLAGS) -Os $(LINKER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	echo Os >> results/benchmark_full.txt
	./$(OUTPUT) >> results/benchmark_full.txt

build:
	mkdir build
