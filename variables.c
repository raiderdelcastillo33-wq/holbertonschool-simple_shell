#include "shell.h"

/**
 * expanded_length - calculates length after special variable expansion
 * @line: original command line
 * @status: previous command status as text
 * @pid: shell process ID as text
 *
 * Return: required length without the terminating null byte
 */
static size_t expanded_length(const char *line, const char *status,
			      const char *pid)
{
	size_t length = 0, index = 0;

	while (line[index] != '\0')
	{
		if (line[index] == '$' && line[index + 1] == '?')
		{
			length += strlen(status);
			index += 2;
		}
		else if (line[index] == '$' && line[index + 1] == '$')
		{
			length += strlen(pid);
			index += 2;
		}
		else
		{
			length++;
			index++;
		}
	}
	return (length);
}

/**
 * expand_variables - replaces $? and $$ in a command line
 * @line: original command line
 * @last_status: status of the previously executed command
 *
 * Return: newly allocated expanded line, or NULL on allocation failure
 */
char *expand_variables(const char *line, int last_status)
{
	char status[32], pid[32];
	char *result, *destination;
	const char *source;
	size_t length;

	sprintf(status, "%d", last_status);
	sprintf(pid, "%ld", (long)getpid());
	length = expanded_length(line, status, pid);
	result = malloc(length + 1);
	if (result == NULL)
		return (NULL);

	source = line;
	destination = result;
	while (*source != '\0')
	{
		if (source[0] == '$' && source[1] == '?')
		{
			strcpy(destination, status);
			destination += strlen(status);
			source += 2;
		}
		else if (source[0] == '$' && source[1] == '$')
		{
			strcpy(destination, pid);
			destination += strlen(pid);
			source += 2;
		}
		else
			*destination++ = *source++;
	}
	*destination = '\0';
	return (result);
}

/**
 * prepare_expanded_arguments - expands a line and parses its arguments
 * @line: original command line
 * @last_status: status of the previously executed command
 * @expanded: address that receives the allocated expanded line
 * @program_name: shell program name for allocation errors
 *
 * Return: parsed arguments, or NULL on failure
 */
char **prepare_expanded_arguments(const char *line, int last_status,
				  char **expanded, char *program_name)
{
	char **args;

	*expanded = expand_variables(line, last_status);
	if (*expanded == NULL)
	{
		perror(program_name);
		return (NULL);
	}
	args = parse_arguments(*expanded);
	if (args == NULL)
	{
		free(*expanded);
		*expanded = NULL;
		perror(program_name);
	}
	return (args);
}

/**
 * execute_expanded_command - resolves and executes expanded arguments
 * @args: parsed argument vector
 * @expanded: allocated expanded command line
 * @program_name: shell program name for error messages
 * @line_number: physical input line number used in error messages
 *
 * Return: command exit status
 */
int execute_expanded_command(char **args, char *expanded,
	char *program_name, int line_number)
{
	char *resolved;
	int status;

	resolved = resolve_command(args[0]);
	if (resolved == NULL)
	{
		fprintf(stderr, "%s: %d: %s: not found\n",
			program_name, line_number, args[0]);
		free(args);
		free(expanded);
		return (127);
	}
	args[0] = resolved;
	status = execute_command(args, program_name);
	free(resolved);
	free(args);
	free(expanded);
	return (status);
}
