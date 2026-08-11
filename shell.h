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
void strip_comment(char *line);
int handle_exit(char **args, char *program_name,
	int last_status, int *exit_status);
int handle_help(char **args, char *program_name);

int handle_setenv(char **args, char *program_name);
int handle_unsetenv(char **args, char *program_name);
void free_shell_env(void);
/**
 * struct alias_s - stores one shell alias
 * @name: alias name
 * @value: alias replacement value
 * @next: next alias in the list
 */
typedef struct alias_s
{
	char *name;
	char *value;
	struct alias_s *next;
} alias_t;

int handle_cd(char **args, char *program_name);
int set_alias_value(const char *name, const char *value);
void print_alias(const char *name);
void print_all_aliases(void);
int handle_alias_line(char *line);
void free_aliases(void);

char *expand_variables(const char *line, int last_status);
char **prepare_expanded_arguments(const char *line, int last_status,
				  char **expanded, char *program_name);
int execute_expanded_command(char **args, char *expanded, char *program_name);

int process_line(char *line, char *program_name, int last_status,
		 int *exit_status);
int process_sequence(char *line, char *program_name, int last_status,
		     int *exit_status);

/**
 * struct history_s - stores one shell history entry
 * @command: stored command
 * @number: visible history number
 * @next: next history entry
 */
typedef struct history_s
{
	char *command;
	unsigned long number;
	struct history_s *next;
} history_t;

int load_history(void);
int add_history(const char *line);
int handle_history(char **args, char *program_name);
int save_history(void);
void free_history(void);
void cleanup_shell(char *line);

#endif
