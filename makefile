PROJECT =   percolation
HEADERS =   $(PROJECT).h
OBJ     =   percolation.o stack.o dfs.o

C99     =   cc -std=c99
CFLAGS  =   -Wall -pedantic -Werror -g

$(PROJECT) : $(OBJ)
	$(C99) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm

%.o : %.c $(HEADERS)
	$(C99) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
