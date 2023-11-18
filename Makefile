CFLAGS = -std=c2x -Wall -Wextra -pedantic
BINARY = lsh

all: main.c
	cc $(CFLAGS) main.c -o $(BINARY)

clean:
	rm -rf $(BINARY)
