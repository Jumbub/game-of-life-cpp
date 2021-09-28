CC = g++

COMPILER_FLAGS = -Wall -std=c++2a
BENCHMARK_FLAGS = -isystem benchmark/include -Lbenchmark/build/src -lbenchmark -lpthread
PROFILE_FLAGS = $(BENCHMARK_FLAGS) -pg

LINKER_FLAGS = -lSDL2
LINKER_FLAGS_TEST = $(LINKER_FLAGS)

OBJ_NAME = build/graphics
OBJ_NAME_TEST = build/test
OBJ_NAME_BENCHMARK = build/benchmark
OBJ_NAME_PROFILE = build/profile

graphics:
	$(CC) src/graphics.cpp $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)
	./$(OBJ_NAME)

benchmark:
	$(CC) src/benchmark.cpp $(BENCHMARK_FLAGS) -o $(OBJ_NAME_BENCHMARK)
	./$(OBJ_NAME_BENCHMARK) > results/benchmark.txt

test:
	$(CC) src/test.cpp $(COMPILER_FLAGS) -ggdb $(LINKER_FLAGS) -o $(OBJ_NAME_TEST)
	./$(OBJ_NAME_TEST)

# profile:
# 	$(CC) src/benchmark.cpp $(PROFILE_FLAGS) -o $(OBJ_NAME_PROFILE)
# 	uftrace record $(OBJ_NAME_PROFILE)
# 	uftrace dump --chrome > results/benchmark.chrome_profile
