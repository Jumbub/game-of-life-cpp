cmake_minimum_required(VERSION 3.8.2)
project(game-of-life-cpp
  LANGUAGES CXX
  VERSION 1.0
)
set(CMAKE_BUILD_TYPE Release)

# Caching
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
endif(CCACHE_FOUND)

# Dependencies
include(FetchContent)
set(SFML_BUILD_AUDIO FALSE)
set(SFML_BUILD_NETWORK FALSE)
if (CMAKE_CXX_COMPILER MATCHES ".*icpx.*")
elseif (CMAKE_CXX_COMPILER MATCHES ".*icpc.*")
  # Need to install SFML manually when using Intel compiler,
  # because I haven't bothered figuring out how to compile SFML separately with GCC
else()
  FetchContent_Declare(
    sfml
    GIT_REPOSITORY "https://github.com/SFML/SFML.git"
    GIT_TAG "2.5.1"
  )
  FetchContent_MakeAvailable(sfml)
endif()
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
  GIT_TAG "v2.13.8"
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
  GIT_TAG "v1.6.1"
)
FetchContent_MakeAvailable(googlebenchmark)

add_subdirectory(src)
