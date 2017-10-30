# HPC-Project1

The   program   will   run,   and   iterate   from   the   minimum   lattice   size   to   the   maximum lattice   size,   doubling   the   lattice   size   each   time,   doing   the   set   amount   of   runs   each time.   Having   a   number   of   runs   equal   to   ~50   will   allow   good   averages   for   speed   to   be achieved.
It   will   also   run   the   program   from   1   to   the   maximum   amount   of   cores   you   specify   for each   run.   This   shows   how   running   the   program   with   more   cores   can   speed   up   or slow   down   the   program.
So   if   the   program   was   told   to   run   from   64   to   128,   with   50   runs   and   8   threads,   it   would run   2   x   50   x   8   =   800   times,   with   half   being   the   64   lattice,   the   other   half   being   the   128 lattice.   This   is   ok   on   smaller   lattice   sizes   as   the   program   is   quite   quick.   However, anything   larger   than   4096   will   take   a   long   time   to   run.

## Usage

1. Compile with `make`
2. Run with `./main <options>`

* Each option has a default value
* None of the options are necessary to run the program

| Option | Usage | Description |
| --- | --- | --- |
| -t | ’s’   OR   ‘b’ | Type   of   lattice   (site   OR   bond) |
| -P | 0-1   float | Chance   of   allocation |
| -p | 0,   1,   OR   2 | whether to consider percolation that goes up-down, left-right, or both |
| -r | positive int | How   many   runs   per   size   for   averaging |
| -n | positive int | Number of threads to use |
| -l | positive even int | Minimum lattice size |
| -h | positive even int | Maximum lattice size |

## Outputs

The   results   of   the   program   will   be   output   to   *site.csv*   or   *bond.csv*,   depending on the type of lattice that was run. They can   be   found   in   the   main   folder   (HPC-Project1).   The csv files show the execution times and speedup of performing percolation and largest cluster tests on different lattice sizes with different amounts of threads. Use `column   -s,   -t   <   site.csv   |   less   -#2   -N   -S` to view the csv files in terminal.
