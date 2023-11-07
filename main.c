#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

const size_t LSH_RL_BUFFERSIZE = 1024;
const size_t LSH_SL_BUFFERSIZE = 64;
const char* LSH_SL_DELIM = " \t\r\n\a";

char* lsh_read_line(void)
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

/*
 * No support for quoting or escaping.
 */
char** lsh_split_line(char* line)
{
    size_t buffer_size = LSH_SL_BUFFERSIZE;
    char** tokens = (char**)malloc(buffer_size * sizeof(char*));

    if (!tokens)
    {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    int position = 0;
    char* token = strtok(line, LSH_SL_DELIM);
    while (token)
    {
        tokens[position] = token;
        ++position;

        if (position >= buffer_size)
        {
            buffer_size += LSH_SL_BUFFERSIZE;
            tokens = realloc(tokens, buffer_size * sizeof(char*));
            if (!tokens)
            {
                fprintf(stderr, "lsh: reallocation error\n");
                exit(EXIT_FAILURE);
            }
        }

        token = strtok(NULL, LSH_SL_DELIM);
    }

    tokens[position] = NULL;
    return tokens;
}

void lsh_loop(void)
{
    int status = 1;
    char* line = NULL;
    char** arguments = NULL;

    do
    {
        printf("> ");
        line = lsh_read_line();
        arguments = lsh_split_line(line);

        free(line); // allocated inside lsh_read_line
        free(arguments); // allocated inside lsh_split_line
    } while (status);
}

int main(int argc, char* argv[])
{
    printf("lsh\n");
    lsh_loop();

    return EXIT_SUCCESS;
}
