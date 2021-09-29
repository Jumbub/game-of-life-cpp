CC = g++

COMPILER_FLAGS = -Wall -std=c++2a
COMPILER_FLAGS_BENCHMARK = $(COMPILER_FLAGS) -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread
COMPILER_FLAGS_BENCHMARK_PROFILE = $(COMPILER_FLAGS_BENCHMARK) -pg

LINKER_FLAGS_GRAPHICS = -lSDL2

OUTPUT = build/interactive
OUTPUT_BENCHMARK = build/benchmark
OUTPUT_BENCHMARK_PROFILE = build/benchmark_profile
OUTPUT_TEST = build/test

OBJS = src/board/next.cpp src/board/random.cpp
OBJS_GRAPHICS = src/board/next.cpp src/board/sdl.cpp src/board/random.cpp

interactive:
	$(CC) src/entrypoints/interactive.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT) $(OBJS_GRAPHICS)
	./$(OUTPUT)

test:
	$(CC) src/entrypoints/test.cpp $(COMPILER_FLAGS) -ggdb -o $(OUTPUT_TEST) $(OBJS)
	./$(OUTPUT_TEST)

benchmark:
	$(CC) src/entrypoints/benchmark.cpp $(COMPILER_FLAGS_BENCHMARK) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT_BENCHMARK) $(OBJS_GRAPHICS)
	./$(OUTPUT_BENCHMARK) > results/benchmark.txt

benchmark_profile: $(OBJS)
	$(CC) src/entrypoints/benchmark.cpp $(COMPILER_FLAGS_BENCHMARK_PROFILE) $(LINKER_FLAGS_GRAPHICS) -o $(OUTPUT_BENCHMARK_PROFILE) $(OBJS_GRAPHICS)
	uftrace record $(OUTPUT_BENCHMARK_PROFILE)
	uftrace dump --chrome > results/benchmark.chrome_profile
