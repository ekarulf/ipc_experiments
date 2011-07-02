CC = clang
LIBS = 

all: fd_bench

fd_bench: fd_bench.o

clean:
	rm -f fd_bench *.o
