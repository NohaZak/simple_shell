#include "simple_shell.h"

/**
 * command_exists - checks if a command exists
 * @cmd: the command to check
 *
 * Return: 1 if the command exists and is executable, 0 otherwise
 */
int command_exists(char* cmd)
{
    return (access(cmd, F_OK | X_OK) == 0);
}

