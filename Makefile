CFLAGS = -std=c2x -Wall -Wextra -pedantic
BINARY = lsh

lsh: lsh.c
	$(CC) $(CFLAGS) lsh.c -o $(BINARY)

clean:
	rm $(BINARY)
