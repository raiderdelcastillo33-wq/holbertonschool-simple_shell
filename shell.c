#include "shell.h"

/**
 * parse_arguments - splits a command line into arguments
 * @line: command line to parse
 *
 * Return: dynamically allocated argument vector, or NULL on failure
 */
char **parse_arguments(char *line)
{
	char **args;
	char **new_args;
	char *token;
	size_t count = 0;
	size_t capacity = 8;

	args = malloc(sizeof(char *) * capacity);
	if (args == NULL)
		return (NULL);

	token = strtok(line, " \t");
	while (token != NULL)
	{
		if (count + 1 >= capacity)
		{
			capacity *= 2;
			new_args = realloc(args, sizeof(char *) * capacity);
			if (new_args == NULL)
			{
				free(args);
				return (NULL);
			}
			args = new_args;
		}
		args[count++] = token;
		token = strtok(NULL, " \t");
	}

	args[count] = NULL;
	return (args);
}

/**
 * execute_command - executes a command with its arguments
 * @args: NULL-terminated argument vector
 * @program_name: shell program name used for error messages
 *
 * Return: child process exit status, or -1 on failure
 */
int execute_command(char **args, char *program_name)
{
	pid_t child;
	int status;

	child = fork();
	if (child == -1)
	{
		perror(program_name);
		return (-1);
	}

	if (child == 0)
	{
		execve(args[0], args, environ);
		perror(program_name);
		_exit(127);
	}

	if (waitpid(child, &status, 0) == -1)
	{
		perror(program_name);
		return (-1);
	}

	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));

	return (-1);
}

/**
 * process_line - parses, resolves and executes one command line
 * @line: command line to process
 * @program_name: shell program name used for errors
 * @last_status: status before processing the current line
 *
 * Return: resulting shell status
 */
static int process_line(char *line, char *program_name, int last_status)
{
	char **args;
	char *resolved;
	int status;

	args = parse_arguments(line);
	if (args == NULL)
	{
		perror(program_name);
		return (1);
	}
	if (args[0] == NULL)
	{
		free(args);
		return (last_status);
	}

	resolved = resolve_command(args[0]);
	if (resolved == NULL)
	{
		fprintf(stderr, "%s: %s: not found\n", program_name, args[0]);
		free(args);
		return (127);
	}

	args[0] = resolved;
	status = execute_command(args, program_name);
	free(resolved);
	free(args);
	return (status);
}

/**
 * main - runs a minimal UNIX command line interpreter
 * @argc: argument count
 * @argv: argument vector
 *
 * Return: status of the last executed command
 */
int main(int argc, char **argv)
{
	char *line = NULL;
	size_t size = 0;
	ssize_t read_count;
	int interactive;
	int last_status = 0;

	(void)argc;
	interactive = isatty(STDIN_FILENO);

	while (1)
	{
		if (interactive)
		{
			printf("#cisfun$ ");
			fflush(stdout);
		}

		read_count = getline(&line, &size, stdin);
		if (read_count == -1)
			break;

		if (read_count > 0 && line[read_count - 1] == '\n')
			line[read_count - 1] = '\0';

		last_status = process_line(line, argv[0], last_status);
	}

	free(line);
	return (last_status);
}
