cmake_minimum_required(VERSION 3.8.2)
project(game-of-speed
  LANGUAGES CXX
  VERSION 1.0
)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_COMPILER_LAUNCHER ccache)

# Dependencies
include(FetchContent)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
FetchContent_Declare(
  sfml
  GIT_REPOSITORY "https://github.com/SFML/SFML.git"
  GIT_TAG "2.5.1"
)
FetchContent_MakeAvailable(sfml)
FetchContent_Declare(
  imgui
  GIT_REPOSITORY "https://github.com/ocornut/imgui.git"
  GIT_TAG "35b1148efb839381b84de9290d9caf0b66ad7d03"
)
FetchContent_MakeAvailable(imgui)
set(IMGUI_DIR ${imgui_SOURCE_DIR})
set(IMGUI_SFML_FIND_SFML OFF)
set(IMGUI_SFML_IMGUI_DEMO OFF)
FetchContent_Declare(
  imgui-sfml
  GIT_REPOSITORY "https://github.com/eliasdaler/imgui-sfml.git"
  GIT_TAG "82dc2033e51b8323857c3ae1cf1f458b3a933c35"
)
FetchContent_MakeAvailable(imgui-sfml)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
  GIT_TAG "v3.0.0-preview3"
)
FetchContent_MakeAvailable(Catch2)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY "https://github.com/google/googletest.git"
  GIT_TAG "v1.10.x"
)
FetchContent_MakeAvailable(googletest)
FetchContent_Declare(
  googlebenchmark
  GIT_REPOSITORY "https://github.com/google/benchmark.git"
  GIT_TAG "master"
)
FetchContent_MakeAvailable(googlebenchmark)


# Compiler flags
set(CommonFlags -Wall -Wextra -Werror -Wpedantic -Wsign-conversion -std=c++2a -O3)
set(DebugFlags -Og -ggdb)
set(ProfileFlags -pg)

# Graphics flags
set(CommonLinks pthread)
set(GraphicsLinks sfml-graphics ImGui-SFML::ImGui-SFML)
set(BenchmarkLinks benchmark::benchmark)
set(TestLinksk Catch2::Catch2WithMain)

# Main
set(MainFlags ${CommonFlags})
set(MainLinks ${CommonLinks} ${GraphicsLinks})
add_executable(main src/entrypoints/main.cpp)
target_link_libraries(main PUBLIC ${MainLinks})
target_compile_options(main PUBLIC ${MainFlags})

# Debug
add_executable(debug src/entrypoints/main.cpp)
target_link_libraries(debug PUBLIC ${MainLinks})
target_compile_options(debug PUBLIC ${MainFlags} ${DebugFlags})

# Profile
add_executable(profile src/entrypoints/main.cpp)
target_link_libraries(profile PUBLIC ${MainLinks})
target_compile_options(profile PUBLIC ${MainFlags} ${ProfileFlags})

# Test
add_executable(test src/entrypoints/test.cpp)
target_link_libraries(test PUBLIC ${CommonLinks} ${TestLinks})

# Benchmark
add_executable(bench src/entrypoints/benchmark.cpp)
target_link_libraries(bench PUBLIC ${MainLinks} ${BenchmarkLinks})
target_compile_options(bench PUBLIC ${MainFlags})

# Benchmark Short
add_executable(bench_short src/entrypoints/benchmark_short.cpp)
target_link_libraries(bench_short PUBLIC ${MainLinks} ${BenchmarkLinks})
target_compile_options(bench_short PUBLIC ${MainFlags})

# All short benchmarks
set(FLAGS "-Ofast" "-O3" "-O2" "-O1" "-O0" "-Os")
foreach(FLAG IN LISTS FLAGS)
  add_executable("bench${FLAG}" src/entrypoints/benchmark_short.cpp)
  target_link_libraries("bench${FLAG}" PUBLIC ${MainLinks} ${BenchmarkLinks})
  target_compile_options("bench${FLAG}" PUBLIC ${MainFlags} ${FLAG})
endforeach()
