# Game of Speed
![tests](https://github.com/jumbub/game-of-speed/actions/workflows/.github/workflows/tests.yaml/badge.svg)
![speed](art/speed.svg)

Conway's Game of Life, as fast as possible.

- [Results of interesting findings](#results-of-interesting-findings)

- [Log of benchmark improvements](#log-of-benchmark-improvements)

- [Running the code](#gettings-started)

![The benchmark world](art/gol.gif)

_(the above gif is the benchmark scenario)_

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

## Log of benchmark improvements

**Benchmark setup:**
- GCC v11.1.0
- Intel(R) Core(TM) i5-7600K CPU @ 3.80GHz (overclocked to 4.4GHz)

**Benchmark conditions:**
- The benchmark value is the average time to generate the "next board" over 10 seconds.
- The first "board" is the same for every test in this log (while performance was slower it was a 500x500 grid, but later into the experiment it became 2560x1440)

### Functioning prototype

Finalised the benchmark board.

[9227c6a55ede200a1b6fe827c93010963e704f3d](https://github.com/Jumbub/game-of-speed/commit/9227c6a55ede200a1b6fe827c93010963e704f3d)

### 525% speed increase in logic

Switching from a 2D vector `std::vector<std::vector<bool>>` to a 1D array `std::unique_pr<new bool[]>`.

[9284f489604e814bb362fe5aa0c5e41ec5158edc](https://github.com/Jumbub/game-of-speed/commit/9284f489604e814bb362fe5aa0c5e41ec5158edc)

### 100% speed increase in logic

Merging and inlining the "finding neighbour positions" and "counting neighbours" logic.

[f64f4adbf7c6f956d2edcff397483de44ccaabec](https://github.com/Jumbub/game-of-speed/commit/f64f4adbf7c6f956d2edcff397483de44ccaabec)

### 250% speed increase in rendering

Rendering the board logic, rather than creating a list of pixels to render.

[cda613a50c4314d087152fb26cf222c76d5ea92d](https://github.com/Jumbub/game-of-speed/commit/cda613a50c4314d087152fb26cf222c76d5ea92d)

### 83% speed increase in logic

Removing the shared\_ptr wrappers, speed > safety.

[73e6448fe2a021d9063bf50323b4b99cd551ee15](https://github.com/Jumbub/game-of-speed/commit/73e6448fe2a021d9063bf50323b4b99cd551ee15)

### 115% speed increase in logic

Reduced the number of neighbour checks from 8 (all) to 3.

Achieved by having a "sliding window" of the neighbour counts for the "left", "middle" and "right" neighbouring columns. When the next cell computes it's neighbours it shifts the window over (so "middle" becomes "left") and only has to generate the new "right" column.

[01cce885bd57f1a5aa4d113a32f4413a61f826a5](https://github.com/Jumbub/game-of-speed/commit/01cce885bd57f1a5aa4d113a32f4413a61f826a5)

### 88% speed increase in logic

Only compute the y levels for the above and below cells once per row.

[9e80c59830588b05ee29c754530fabf50f51a9e0](https://github.com/Jumbub/game-of-speed/commit/9e80c59830588b05ee29c754530fabf50f51a9e0)

### ∞% speed increase in rendering

Segmented the rendering and computation into separate threads. _At this stage_ the rendering takes less time than the computation, so effectively I've reduced render time to 0.

[2619b4164bd2fca3fd8b30dca22cd83a75fe43b9](https://github.com/Jumbub/game-of-speed/commit/2619b4164bd2fca3fd8b30dca22cd83a75fe43b9)

### THREADS% speed increase in logic

Finally gave in to the lazy performance improvement, throwing more threads at the problem.

[2f68783cc8a3d9bd7590e617b71a5a0df010af56](https://github.com/Jumbub/game-of-speed/commit/2f68783cc8a3d9bd7590e617b71a5a0df010af56)

### 25% speed increase in logic

When converting values of `0`/`maxInt` to `0`/`1` we now use a mathematical expression `1 - (state + 1)` instead of a ternary.

[21bd48749570ac9c05663c661f60ce5ede1223ff](https://github.com/Jumbub/game-of-speed/commit/21bd48749570ac9c05663c661f60ce5ede1223ff)

### 70% speed increase in logic

Insane free performance improvement by passing the optimization flag `-Ofast`.

[21761f93075a7aa26c314ae675b92df07c5f5892](https://github.com/Jumbub/game-of-speed/commit/21761f93075a7aa26c314ae675b92df07c5f5892)

### 1% speed increase in logic

Enabling the compiler flag `-Wsign-conversion` to find and remove any unecessary signed to unsigned int conversions.

[86b3634383c6bd2d2f4104c441b2a2328f2bf6f8](https://github.com/Jumbub/game-of-speed/commit/86b3634383c6bd2d2f4104c441b2a2328f2bf6f8)

### 15% speed increase in logic, for 100% speed decrease in rendering

For now I am going to focus on the logic performance instead of the main loop performance. The first step towards that is removing the "render" centric logic which was storing cells as "pixel friendly" values.

If at some point I can figure out how to tell SDL to render 0/1 byte values, I will gladly do so, and the render performance will be restored.

[b075a2f68ded65e3828d18e972e9a0fd0a24d596](https://github.com/Jumbub/game-of-speed/commit/b075a2f68ded65e3828d18e972e9a0fd0a24d596)

### ~~100% speed increase in logic~~

_if you only do half the work, you're twice as quick_

Commit with "improvement" [2a807cb95703824189e2ede051b4c1621179a4c1](https://github.com/Jumbub/game-of-speed/commit/2a807cb95703824189e2ede051b4c1621179a4c1)

Changing from `height * height` back to `width * height` puts me back to the previous speed.

[4f0bf37b1394a90307e148033f0602050e668b4e](https://github.com/Jumbub/game-of-speed/commit/4f0bf37b1394a90307e148033f0602050e668b4e)

## Results of interesting findings

Bear in mind these findings were made on **my** computer(details of which are [here](#log-of-benchmark-improvements)), and may not be applicable to your machine or code.

### Leveraging the speed of sequential memory access (+10% speed)

Commit [here.](https://github.com/Jumbub/game-of-speed/commit/234acceeb7817f8da738f327afa7a18eaa5806ac)

**Context:**
- To calculate a new cell state, you need to count the number of alive neighbours
- To count the number of alive neighbours, you need to retrieve their states from memory
- The memory is stored in a flat array of size `width*height`

**Base code:**
- Keeps track of 3 offsets to use on the `input` array

```
unsigned int yAboveBase = 0;
unsigned int yBelowBase = 0;
unsigned int yBase = 0;

neighbours[2] = input[yBelowBase + nextX] + input[yBase + nextX] + input[yAboveBase + nextX];
```

**10% faster code:**
- Creates 3 arrays which have their base offsets assigned to the `0` index

```
unsigned int *neighboursBelow neighboursBelow = &input[lastYBase];
unsigned int *neighboursMiddle neighboursMiddle = &input[middleYBase];
unsigned int *neighboursAbove neighboursAbove = &input[nextYBase];

neighbours[2] = neighboursBelow[nextX] + neighboursMiddle[nextX] + neighboursAbove[nextX];
```

I _assume_ the increase in performance is through the change to access memory being sequentially.

### Compiler optimizations != CPU optimizations

Commit [here.](https://github.com/Jumbub/game-of-speed/commit/38412c067a9d70cbad8ed20d9a693cdb9074be0c)

I had initially enabled the "fastest" compiler optimization flag in [this](https://github.com/Jumbub/game-of-speed/commit/21761f93075a7aa26c314ae675b92df07c5f5892) commit, with the assumption that tangible improvements in performance could only be made by making changes to the code (in this case by "optimizing" it).

What I have seen is that some "optimizations" in code may actually negatively impact performance because it limits the CPUs ability to perform _it's_ own runtime optimizations. It is my assumption that in the case of this code base, leaving in the extra code as "hints" to the CPU is actually allowing it to improve internal branch prediction and caching strategies.

Here are **my** results for each optimization flag provided by gcc:

```
-Ofast : 4.42ms (2nd)
-O3    : 4.54ms (4th)
-O2    : 4.52ms (3rd)
-O1    : 4.28ms (the best)
-O0    : 8.33ms (6th)
-Os    : 4.65ms (5th)
```

### Which `0`/`MAX_INT` to `0`/`1` conversion is faster?

Code at the time of this experiment [here.](https://github.com/Jumbub/game-of-speed/commit/bc10bbc88ff9c17a27b0dcff030bf221487ca893#diff-4d7b1c4aec6e6dda9ec986006afcec04363acf48f874a0e1c1caa2fff15481d1)

```
// the best
inline const auto math(unsigned int n) {
  return (1 - (n + 1));
}

// 3.3% slower
inline const auto mask(unsigned int n) {
  return n & 1;
}

// 3.3% slower
inline const auto shift(unsigned int n) {
  return n >> 31;
}

// 30% slower
inline const auto bool(unsigned int n) {
  return n == UINT32_MAX;
}
```

After taking a look at the assembly, it turns out the first function compiles to the [_neg_](https://www.ibm.com/docs/it/aix/7.2?topic=set-neg-negate-instruction) instruction as opposed to the _and_ or _shift_ instructions. I guess _neg_ is just an easier computation.

### Which `if` condition is faster?

Code at the time of this experiment [here.](https://github.com/Jumbub/game-of-speed/tree/44a384ece1a2376c991735c3ab4802b44ba15b8c)

```
// 4.51ms: the best
if (currentStateBool && (totalNeighbours < 2 || totalNeighbours > 3))
  output[i] = DEAD;
else if (!currentStateBool && totalNeighbours == 3)
  output[i] = ALIVE;
else
  output[i] = currentStateBool;

// 4.89ms: 8.5% slower
if (totalNeighbours == 3)
  output[i] = ALIVE;
else if (totalNeighbours < 2 || totalNeighbours > 3)
  output[i] = DEAD;
else
  output[i] = currentStateBool;

// 5.07ms: 12.5% slower
if (totalNeighbours < 2 || totalNeighbours > 3)
  output[i] = DEAD;
else if (totalNeighbours == 3)
  output[i] = ALIVE;
else
  output[i] = currentStateBool;
```

I had assumed that the 3rd case would actually be the fastest, because _most_ cells are dead and have less than 2 neighbours.

My only vague idea about how this first solution could be faster is that it is able to do better branch prediction.


## References

Very good utility for visualising assembly instructions of your program:
[https://godbolt.org/](https://godbolt.org/)

Great util for generating binary image files:
[https://www.dcode.fr/binary-image](https://www.dcode.fr/binary-image)
