#include "shell.h"

/**
 * copy_string - creates a dynamically allocated string copy
 * @source: string to copy
 *
 * Return: allocated copy, or NULL on failure
 */
static char *copy_string(const char *source)
{
	char *copy;

	copy = malloc(strlen(source) + 1);
	if (copy == NULL)
		return (NULL);

	strcpy(copy, source);
	return (copy);
}

/**
 * build_path - builds a full executable path
 * @directory: PATH directory
 * @command: command name
 *
 * Return: allocated full path, or NULL on failure
 */
static char *build_path(const char *directory, const char *command)
{
	char *candidate;
	size_t length;

	length = strlen(directory) + strlen(command) + 2;
	candidate = malloc(length);
	if (candidate == NULL)
		return (NULL);

	strcpy(candidate, directory);
	strcat(candidate, "/");
	strcat(candidate, command);

	return (candidate);
}

/**
 * search_path - searches a command inside PATH directories
 * @path: writable copy of PATH
 * @command: command to find
 *
 * Return: allocated executable path, or NULL if not found
 */
static char *search_path(char *path, char *command)
{
	char *directory;
	char *end;
	char *candidate;
	char separator;

	directory = path;
	while (*directory != '\0')
	{
		while (*directory == ':')
			directory++;
		if (*directory == '\0')
			break;
		end = directory;
		while (*end != '\0' && *end != ':')
			end++;
		separator = *end;
		*end = '\0';
		candidate = build_path(directory, command);
		if (candidate == NULL)
			return (NULL);
		if (access(candidate, X_OK) == 0)
			return (candidate);
		free(candidate);
		if (separator == '\0')
			break;
		directory = end + 1;
	}
	return (NULL);
}

/**
 * resolve_command - resolves a command to an executable path
 * @command: command name or path
 *
 * Return: allocated executable path, or NULL if not found
 */
char *resolve_command(char *command)
{
	char *path_copy;
	char *resolved;
	int index = 0;

	if (strchr(command, '/') != NULL)
	{
		if (access(command, X_OK) == -1)
			return (NULL);
		return (copy_string(command));
	}

	while (environ[index] != NULL &&
	       strncmp(environ[index], "PATH=", 5) != 0)
		index++;

	if (environ[index] == NULL)
		return (NULL);

	path_copy = copy_string(environ[index] + 5);
	if (path_copy == NULL)
		return (NULL);

	resolved = search_path(path_copy, command);
	free(path_copy);

	return (resolved);
}
