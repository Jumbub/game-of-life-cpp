# Common flags & links
set(Flags -Wall -Wextra -Werror -Wpedantic -Wsign-conversion -std=c++2a -O3)
set(LinksNoGraphics pthread)
set(Links ${LinksNoGraphics} sfml-graphics ImGui-SFML::ImGui-SFML)

# Files
file(GLOB GraphicsFiles CONFIGURE_DEPENDS "graphics/*.cpp")
set(MainFiles ${GraphicsFiles})

# Main
add_executable(main main.cpp ${MainFiles})
target_link_libraries(main PUBLIC ${Links} ${Graphics})
target_compile_options(main PUBLIC ${Flags})

# Debug
add_executable(debug main.cpp ${MainFiles})
target_link_libraries(debug PUBLIC ${Links} ${Graphics})
target_compile_options(debug PUBLIC ${Flags} -Og -ggdb)

# Test
add_executable(test test/main.cpp)
target_link_libraries(test PUBLIC ${LinksNoGraphics} Catch2::Catch2WithMain)

# Benchmark
set(Benchmark benchmark::benchmark)
add_executable(bench benchmark/main.cpp ${MainFiles})
target_link_libraries(bench PUBLIC ${Links} ${Benchmark})
target_compile_options(bench PUBLIC ${Flags})

# Quick benchmark
add_executable(bench_short benchmark/main_short.cpp)
target_link_libraries(bench_short PUBLIC ${Links} ${Benchmark})
target_compile_options(bench_short PUBLIC ${Flags})

# Permutations of quick benchmarks
set(flag_overrides "-Ofast" "-O3" "-O2" "-O1" "-O0" "-Os")
foreach(flag_override IN LISTS flag_overrides)
  add_executable("bench${flag_override}" benchmark/main_short.cpp)
  target_link_libraries("bench${flag_override}" PUBLIC ${Links} ${Benchmark})
  target_compile_options("bench${flag_override}" PUBLIC ${Flags} ${flag_override})
endforeach()
