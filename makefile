PROJECT =   main
HEADERS =   $(PROJECT).h
OBJ     =   main.o percolation.o clusterThread.o clusterNode.o stack.o dfs.o dfsThread.o queue.o floodfill.o floodfillThread.o siteTimers.o bondTimers.o lattice.o unionFind.o options.o

C99     =   mpicc -std=c99 -fopenmp
CFLAGS  =   -Wall -pedantic -Werror -g

$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm
	rm -f run.sh.*
	syncCluster

%.o : %.c $(HEADERS)
	$(C99) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
