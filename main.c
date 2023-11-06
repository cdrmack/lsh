#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

const size_t LSH_RL_BUFFERSIZE = 1024;

char* lsh_read_line()
{
    char* buffer = NULL;
    size_t buffer_size = 0;

    if (getline(&buffer, &buffer_size, stdin) == -1)
    {
        if (feof(stdin))
        {
            exit(EXIT_SUCCESS);
        }

        if (ferror(stdin))
        {
            fprintf(stderr, "lsh: getline error\n");
            exit(EXIT_FAILURE);
        }
    }

    return buffer;
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
