# HPC-Project1

The purpose of this program is to simulate site and bond percolation models for a 2-dimensional square lattice. The program uses OpenMP to parallelise processes across multiple threads and MPI to parallelise processes across multiple nodes in a cluster.

## Lattice

The 2-dimensional square lattice has a regular grid structure with wrap-around connections. Each grid point has four neighbours, east, west, north and south. The wrap-around connections are necessary for grid points on the boundaries of the lattice. For example, the west neighbour of a grid point on the left boundary is the grid point in the same row on the right boundary. Similarly, the north neighbour of a grid point on the top boundary is the grid point in the same column on the bottom boundary.

## Site percolation

Each point on a lattice is considered to be 'occupied' with a probablity p and unoccupied with a probability 1-p. Randomly generating a lattice with occupied and unoccupied sites is called seeding. Once the lattice is seeded, a bond is assumed to form between two neighbouring lattice sites P and Q, if both are occupied. A cluster of lattice sites is defined as a collection of sites such that given a pair of sites within the cluster, it is possible to reach one from the other by following the bonds. The lattice percolates if there is a cluster that spans the entire lattice. More clearly:

* Left-right percolation: a cluster that spans all the rows;
* Up-down percolation: a cluster that spans all the columns;

## Bond percolation

The seeding process is the key difference between bond and site percolation. In bond percolation, it is the bonds between neighboring lattice sites that are seeded; instead of seeding the occupancy of a lattice site. The notion of clusters and percolation are otherwise the same.

## Simulations

The program will run, and iterate from a minimum lattice size to a maximum lattice size, doubling the lattice size each time, doing the set amount of runs each time.

It will also run the program from 1 to the maximum amount of cores you specify for each run. This shows how running the program with more cores can speed up or slow down the program.
So if the program was told to run from a lattice size of 64 to 128, with 50 runs per lattice size and using 8 threads, it would run 2 x 50 x 8 = 800 times, with half of the runs being the 64 lattice, the other half being the 128 lattice. This is ok on smaller lattice sizes as the program is quick, but a perfoming simulations on lattice size larger than 4096 will take a long time to run.

## Usage

1. Compile with `make`
2. Run with `./main <options>`

* Each option has a default value
* None of the options are necessary to run the program

| Option | Usage | Description |
| --- | --- | --- |
| -l | ’s’ OR ‘b’ | Type of lattice (site OR bond) |
| -P | 0-1 float | Chance of allocation |
| -p | 0, 1, OR 2 | Whether to consider percolation that goes up-down, left-right, or both |
| -r | positive int | How many runs per size for averaging |
| -t | positive int | Number of threads per node to use |
| -n | positive int | Number of nodes to use |
| -m | positive even int | Minimum lattice size |
| -M | positive even int | Maximum lattice size |

## Outputs

The results of the program will be outputted to *site.csv* or *bond.csv*, depending on the type of lattice that was run. They can be found in the main folder (HPC-Project1). The csv files show the execution times and speedup of performing percolation and largest cluster tests on different lattice sizes with different amounts of threads. Use `column -s, -t < site.csv | less -#2 -N -S` to view the csv files in terminal.
