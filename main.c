#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

const size_t LSH_RL_BUFFERSIZE = 1024;
const size_t LSH_SL_BUFFERSIZE = 64;
const char* LSH_SL_DELIM = " \t\r\n\a";

// some commands needs to be builtin
int lsh_cd(char** args);
int lsh_exit(char** args);
int lsh_help(char** args);

char* builtin_str[] = {
    "help",
    "cd",
    "exit",
};

// array of function pointers that return int and take char**
int (*builtin_func[])(char**) = {
    &lsh_help,
    &lsh_cd,
    &lsh_exit,
};

int lsh_num_builtins(void) {
    return sizeof(builtin_str) / sizeof(char*);
}

int lsh_help(char** args)
{
    printf("The following functions are builtin:\n");
    for (int i = 0; i < lsh_num_builtins(); ++i)
    {
        printf("  %s\n", builtin_str[i]);
    }

    return 1;
}

int lsh_cd(char** args)
{
    if (args[1] == NULL)
    {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
        return 0;
    }

    if (chdir(args[1]) != 0)
    {
        perror("lsh");
    }

    return 1;
}

int lsh_exit(char** args)
{
    exit(EXIT_SUCCESS);
}

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

// no support for quoting or escaping
char** lsh_split_line(char* line)
{
    size_t buffer_size = LSH_SL_BUFFERSIZE;
    char** tokens = (char**)malloc(buffer_size * sizeof(char*));

    if (!tokens)
    {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }

    size_t position = 0;
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

int lsh_launch(char** args)
{
    int status = 0;
    pid_t pid;

    pid = fork();
    if (pid < 0 )
    {
        // error
        perror("lsh");
    }
    else if (pid == 0)
    {
        // child
        if (execvp(args[0], args) == -1)
        {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    }
    else
    {
        // parent
        pid_t wpid;
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

int lsh_execute(char** args)
{
    if (args[0] == NULL)
    {
        // empty command
        return 1;
    }

    for (int i = 0; i < lsh_num_builtins(); ++i)
    {
        if (strcmp(args[0], builtin_str[i]) == 0)
        {
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args);
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
        lsh_execute(arguments);

        free(line); // allocated inside lsh_read_line
        free(arguments); // allocated inside lsh_split_line
    } while (status);
}

int main(void)
{
    printf("lsh\n");
    lsh_loop();

    return EXIT_SUCCESS;
}
