# Game of Speed

Conway's Game of Life as fast as possible.

![](art/gol.png)

(Pictured above is the "benchmark" world)

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

## Milestones

### 40ms -> 23ms

Merging and inlining the "finding neighbour positions" and "counting neighbours" logic.

[f64f4adbf7c6f956d2edcff397483de44ccaabec](https://github.com/Jumbub/game-of-speed/commit/f64f4adbf7c6f956d2edcff397483de44ccaabec)

### 250ms -> 40ms

Switching from a 2D vector `std::vector<std::vector<bool>>` to a 1D array `std::unique_pr<new bool[]>`.

[9284f489604e814bb362fe5aa0c5e41ec5158edc](https://github.com/Jumbub/game-of-speed/commit/9284f489604e814bb362fe5aa0c5e41ec5158edc)

### Functioning prototype

Finalised the benchmark board.

[9227c6a55ede200a1b6fe827c93010963e704f3d](https://github.com/Jumbub/game-of-speed/commit/9227c6a55ede200a1b6fe827c93010963e704f3d)

## References

Great util for generating binary image files:
https://www.dcode.fr/binary-image
