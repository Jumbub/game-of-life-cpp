# Game of Speed

Conway's Game of Life, as fast as possible.

![tests](https://github.com/jumbub/game-of-speed/actions/workflows/.github/workflows/tests.yaml/badge.svg)
![speed](art/speed.svg)

![The benchmark world](art/gol.png)

_(pictured above is the benchmark scenario)_

## Gettings started

### Running the app

`make`

Requires:

- https://www.libsdl.org/

### Running the tests

`make test`

Requires:

- https://github.com/catchorg/Catch2

### Running a benchmark

`make benchmark`

Requires:

- https://github.com/google/benchmark

## The journey

### 101% speed increase in logic

Enabling the compiler flag `-Wsign-conversion` to find and remove any unecessary signed to unsigned int conversions.

[86b3634383c6bd2d2f4104c441b2a2328f2bf6f8](https://github.com/Jumbub/game-of-speed/commit/86b3634383c6bd2d2f4104c441b2a2328f2bf6f8)

### 170% speed increase in logic

Insane free performance improvement by passing the optimization flag `-Ofast`.

[21761f93075a7aa26c314ae675b92df07c5f5892](https://github.com/Jumbub/game-of-speed/commit/21761f93075a7aa26c314ae675b92df07c5f5892)

### 125% speed increase in logic

When converting values of `0`/`maxInt` to `0`/`1` we now use a mathematical expression `1 - (state + 1)` instead of a ternary.

[21bd48749570ac9c05663c661f60ce5ede1223ff](https://github.com/Jumbub/game-of-speed/commit/21bd48749570ac9c05663c661f60ce5ede1223ff)

### THREADS% speed increase in logic

Finally gave in to the lazy performance improvement, throwing more threads at the problem.

[2f68783cc8a3d9bd7590e617b71a5a0df010af56](https://github.com/Jumbub/game-of-speed/commit/2f68783cc8a3d9bd7590e617b71a5a0df010af56)

### ∞% speed increase in rendering

Segmented the rendering and computation into separate threads. _At this stage_ the rendering takes less time than the computation, so effectively I've reduced render time to 0.

[2619b4164bd2fca3fd8b30dca22cd83a75fe43b9](https://github.com/Jumbub/game-of-speed/commit/2619b4164bd2fca3fd8b30dca22cd83a75fe43b9)

### 188% speed increase in logic

Only compute the y levels for the above and below cells once per row.

[9e80c59830588b05ee29c754530fabf50f51a9e0](https://github.com/Jumbub/game-of-speed/commit/9e80c59830588b05ee29c754530fabf50f51a9e0)

### 215% speed increase in logic

Reduced the number of neighbour checks from 8 (all) to 3.

Achieved by having a "sliding window" of the neighbour counts for the "left", "middle" and "right" neighbouring columns. When the next cell computes it's neighbours it shifts the window over (so "middle" becomes "left") and only has to generate the new "right" column.

[01cce885bd57f1a5aa4d113a32f4413a61f826a5](https://github.com/Jumbub/game-of-speed/commit/01cce885bd57f1a5aa4d113a32f4413a61f826a5)

### 183% speed increase in logic

Removing the shared\_ptr wrappers, speed > safety.

[73e6448fe2a021d9063bf50323b4b99cd551ee15](https://github.com/Jumbub/game-of-speed/commit/73e6448fe2a021d9063bf50323b4b99cd551ee15)

### 350% speed increase in rendering

Rendering the board logic, rather than creating a list of pixels to render.

[cda613a50c4314d087152fb26cf222c76d5ea92d](https://github.com/Jumbub/game-of-speed/commit/cda613a50c4314d087152fb26cf222c76d5ea92d)

### 200% speed increase in logic

Merging and inlining the "finding neighbour positions" and "counting neighbours" logic.

[f64f4adbf7c6f956d2edcff397483de44ccaabec](https://github.com/Jumbub/game-of-speed/commit/f64f4adbf7c6f956d2edcff397483de44ccaabec)

### 625% speed increase in logic

Switching from a 2D vector `std::vector<std::vector<bool>>` to a 1D array `std::unique_pr<new bool[]>`.

[9284f489604e814bb362fe5aa0c5e41ec5158edc](https://github.com/Jumbub/game-of-speed/commit/9284f489604e814bb362fe5aa0c5e41ec5158edc)

### Functioning prototype

Finalised the benchmark board.

[9227c6a55ede200a1b6fe827c93010963e704f3d](https://github.com/Jumbub/game-of-speed/commit/9227c6a55ede200a1b6fe827c93010963e704f3d)

## Findings

Bear in mind the following results pertain specifically to the context of the code at the time.

### What's the fastest way to convert `0`/`MAX_INT` to `0`/`1` respectively?

Code at the time of this experiment [here.](https://github.com/Jumbub/game-of-speed/commit/bc10bbc88ff9c17a27b0dcff030bf221487ca893#diff-4d7b1c4aec6e6dda9ec986006afcec04363acf48f874a0e1c1caa2fff15481d1)

```
inline const auto maxToOne(Cell n) { // the best
  return (1 - (n + 1));
}
inline const auto maxToOne(Cell n) { // 3.3% slower total benchmark time
  return n & 1;
}
inline const auto maxToOne(Cell n) { // 3.3% slower total benchmark time
  return n >> 31;
}
inline const auto maxToOne(Cell n) { // 30% slower total benchmark time
  return n == UINT32_MAX;
}
```

## References

Great util for generating binary image files:
https://www.dcode.fr/binary-image
