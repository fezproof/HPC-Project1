PROJECT =   percolation
HEADERS =   $(PROJECT).h
OBJ     =   percolation.o stack.o dfs.o dfsThread.o queue.o floodfill.o

C99     =   gcc-7 -std=c99 -fopenmp
CFLAGS  =   -Wall -pedantic -Werror -g

$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm

%.o : %.c $(HEADERS)
	$(C99) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
