#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const int LSH_RL_BUFFERSIZE = 1024;

char* lsh_read_line()
{
    int buffer_size = LSH_RL_BUFFERSIZE;
    char* buffer = malloc(sizeof(char) * buffer_size);

    if (!buffer)
    {
        fprintf(stderr, "lsh: allocation error\n");
	exit(EXIT_FAILURE);
    }

    int position = 0;
    while (true)
    {
        char c = getchar();

	if (c == EOF || c == '\n')
        {
  	    buffer[position] = '\n';
	    return buffer;
        }
	else
        {
  	    buffer[position] = c;
	}
	
	++position;

	if (position >= buffer_size)
        {
	    // TODO: realocate buffer if needed
	}
    }
}

void lsh_loop(void)
{
    char* line;
    int status = 1;

    do
    {
        printf("> ");
	line = lsh_read_line();
        printf("read: %s", line); // TODO: remove

	free(line); // allocated inside lsh_read_line
    } while (status);
}

int main(int argc, char* argv[])
{
    printf("lsh\n");
    lsh_loop();

    return EXIT_SUCCESS;
}
