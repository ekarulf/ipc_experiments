CC = clang
OS := $(shell uname -s | tr "[:upper:]" "[:lower:]")
LIBS = 

ifeq ($(OS), linux)
LIBS += strlcat.o strlcpy.o
endif

all: fd_bench

fd_bench: fd_bench.o $(LIBS)

clean:
	rm -f fd_bench $(LIBS)
