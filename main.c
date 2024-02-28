#include "simple_shell.h"

/**
 * main - entry point for the simple shell
 * @argc: argument count
 * @argv: argument vector
 * @envp: environment variables
 *
 * Return: 0 on success, 1 on failure
 */
int main(int argc, char *argv[], char *envp[])
{
    /* To avoid unused parameter warning */
    (void)argc;
    (void)argv;

    char *cmd;
    size_t cmd_len = 0;
    char **env;
    char* args[MAX_NUM_ARGS + 1];
    char* token;
    int i;
    int pid;

    cmd = NULL;

    if (isatty(STDIN_FILENO))
    {
        while (1)
        {
            printf("($) ");
            if (getline(&cmd, &cmd_len, stdin) == -1)
            {
                printf("\n");
                exit(0);
            }

            cmd[strlen(cmd) - 1] = '\0'; /* Remove newline character */

            if (strlen(cmd) == 0)
            {
                continue;
            }

            /* Check for 'exit' command */
            if (strcmp(cmd, "exit") == 0)
            {
                free(cmd);
                exit(0);
            }

            /* Check for 'env' command */
            if (strcmp(cmd, "env") == 0)
            {
                for (env = envp; *env != 0; env++)
                {
                    char *thisEnv = *env;
                    printf("%s\n", thisEnv);
                }
                continue;
            }

            /* Split command into arguments */
            token = strtok(cmd, " ");
            i = 0;
            while (token != NULL && i < MAX_NUM_ARGS)
            {
                args[i] = token;
                token = strtok(NULL, " ");
                i++;
            }
            args[i] = NULL; /* Last element should be NULL for execvp */

            if (!command_exists(args[0]))
            {
                fprintf(stderr, "%s: command not found\n", args[0]);
                continue;
            }

            pid = fork();

            if (pid == 0) /* Child process */
            {
                execve(args[0], args, envp);
                perror("execvp failed");
                return (1);
            }
            else if (pid < 0) /* Fork failed */
            {
                perror("fork failed");
                return (1);
            }
            else /* Parent process */
            {
                wait(NULL);
            }
        }
    }
    else /* Non-interactive mode */
    {
        while (getline(&cmd, &cmd_len, stdin) != -1)
        {
            cmd[strlen(cmd) - 1] = '\0'; /* Remove newline character */

            /* Split command into arguments */
            token = strtok(cmd, " ");
            i = 0;
            while (token != NULL && i < MAX_NUM_ARGS)
            {
                args[i] = token;
                token = strtok(NULL, " ");
                i++;
            }
            args[i] = NULL; /* Last element should be NULL for execvp */

            if (!command_exists(args[0]))
            {
                fprintf(stderr, "%s: command not found\n", args[0]);
                continue;
            }

            pid = fork();

            if (pid == 0) /* Child process */
            {
                execve(args[0], args, envp);
                perror("execvp failed");
                return (1);
            }
            else if (pid < 0) /* Fork failed */
            {
                perror("fork failed");
                return (1);
            }
            else /* Parent process */
            {
                wait(NULL);
            }
        }
    }

    free(cmd);
    return (0);
}

