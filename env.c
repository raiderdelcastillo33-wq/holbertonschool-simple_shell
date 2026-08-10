#include "shell.h"

static int env_is_private;

/**
 * find_env_index - finds an environment variable by name
 * @name: variable name
 *
 * Return: index of variable, or -1 if not found
 */
static int find_env_index(const char *name)
{
	size_t length;
	int index;

	length = strlen(name);
	for (index = 0; environ[index] != NULL; index++)
	{
		if (strncmp(environ[index], name, length) == 0 &&
		    environ[index][length] == '=')
			return (index);
	}
	return (-1);
}

/**
 * ensure_private_env - creates a shell-owned environment copy
 *
 * Return: 0 on success, -1 on allocation failure
 */
static int ensure_private_env(void)
{
	char **copy;
	int count = 0, index, cleanup;

	if (env_is_private)
		return (0);
	while (environ[count] != NULL)
		count++;
	copy = malloc(sizeof(char *) * (count + 1));
	if (copy == NULL)
		return (-1);
	for (index = 0; index < count; index++)
	{
		copy[index] = malloc(strlen(environ[index]) + 1);
		if (copy[index] == NULL)
		{
			for (cleanup = 0; cleanup < index; cleanup++)
				free(copy[cleanup]);
			free(copy);
			return (-1);
		}
		strcpy(copy[index], environ[index]);
	}
	copy[count] = NULL;
	environ = copy;
	env_is_private = 1;
	return (0);
}

/**
 * handle_setenv - creates or updates an environment variable
 * @args: command arguments
 * @program_name: shell program name
 *
 * Return: 0 on success, 1 on error
 */
int handle_setenv(char **args, char *program_name)
{
	char *entry, **new_env;
	size_t name_len, value_len;
	int index, count, i;

	if (args[1] == NULL || args[2] == NULL || args[3] != NULL ||
	    args[1][0] == '\0' || strchr(args[1], '=') != NULL)
		return (fprintf(stderr, "%s: setenv: invalid arguments\n",
			       program_name), 1);
	if (ensure_private_env() == -1)
		return (perror(program_name), 1);
	name_len = strlen(args[1]);
	value_len = strlen(args[2]);
	entry = malloc(name_len + value_len + 2);
	if (entry == NULL)
		return (perror(program_name), 1);
	sprintf(entry, "%s=%s", args[1], args[2]);
	index = find_env_index(args[1]);
	if (index >= 0)
	{
		free(environ[index]);
		environ[index] = entry;
		return (0);
	}
	for (count = 0; environ[count] != NULL; count++)
		;
	new_env = malloc(sizeof(char *) * (count + 2));
	if (new_env == NULL)
		return (free(entry), perror(program_name), 1);
	for (i = 0; i < count; i++)
		new_env[i] = environ[i];
	new_env[count] = entry;
	new_env[count + 1] = NULL;
	free(environ);
	environ = new_env;
	return (0);
}

/**
 * handle_unsetenv - removes an environment variable
 * @args: command arguments
 * @program_name: shell program name
 *
 * Return: 0 on success, 1 on error
 */
int handle_unsetenv(char **args, char *program_name)
{
	int index, next;

	if (args[1] == NULL || args[2] != NULL || args[1][0] == '\0' ||
	    strchr(args[1], '=') != NULL)
	{
		fprintf(stderr, "%s: unsetenv: invalid arguments\n", program_name);
		return (1);
	}
	index = find_env_index(args[1]);
	if (index == -1)
		return (0);
	if (ensure_private_env() == -1)
	{
		perror(program_name);
		return (1);
	}
	index = find_env_index(args[1]);
	free(environ[index]);
	for (next = index; environ[next] != NULL; next++)
		environ[next] = environ[next + 1];
	return (0);
}

/**
 * free_shell_env - frees the shell-owned environment
 */
void free_shell_env(void)
{
	int index;

	if (!env_is_private)
		return;
	for (index = 0; environ[index] != NULL; index++)
		free(environ[index]);
	free(environ);
	environ = NULL;
	env_is_private = 0;
}
