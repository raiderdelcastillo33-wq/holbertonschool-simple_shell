#ifndef SHELL_H
#define SHELL_H

#define SHELL_EXIT 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

extern char **environ;

char **parse_arguments(char *line);
char *resolve_command(char *command);
int execute_command(char **args, char *program_name);
void install_shell_sigint(void);
void ignore_shell_sigint(void);

ssize_t shell_getline(char **line);
int handle_exit(char **args, char *program_name,
	int last_status, int *exit_status);

int handle_setenv(char **args, char *program_name);
int handle_unsetenv(char **args, char *program_name);
void free_shell_env(void);

#endif
