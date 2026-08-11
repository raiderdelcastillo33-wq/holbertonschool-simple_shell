#include "shell.h"
#include <fcntl.h>

static history_t *history_head;
static history_t *history_tail;
static unsigned long history_next;

/**
 * add_history - adds one command to history
 * @line: command to store
 *
 * Return: 0 on success, 1 on failure
 */
int add_history(const char *line)
{
	history_t *node;
	size_t length, index;

	if (line == NULL || line[0] == '\0')
		return (0);
	node = malloc(sizeof(*node));
	if (node == NULL)
		return (1);
	length = strlen(line);
	node->command = malloc(length + 1);
	if (node->command == NULL)
		return (free(node), 1);
	for (index = 0; index <= length; index++)
		node->command[index] = line[index];
	node->number = history_next;
	node->next = NULL;
	history_next = (history_next + 1) % 4096;
	if (history_tail == NULL)
		history_head = node;
	else
		history_tail->next = node;
	history_tail = node;
	return (0);
}

/**
 * load_history - loads commands from the history file
 *
 * Return: 0 on success or absent file, 1 on failure
 */
int load_history(void)
{
	char *home = NULL, path[4096], line[4096], c;
	size_t i = 0, len = 0;
	int fd;
	ssize_t count;

	while (environ[i] != NULL && home == NULL)
	{
		if (strncmp(environ[i], "HOME=", 5) == 0)
			home = environ[i] + 5;
		i++;
	}
	if (home == NULL ||
	    strlen(home) + strlen("/.simple_shell_history") >= sizeof(path))
		return (1);
	strcpy(path, home);
	strcat(path, "/.simple_shell_history");
	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (0);
	while ((count = read(fd, &c, 1)) > 0)
	{
		if (c == '\n' || len == sizeof(line) - 1)
		{
			line[len] = '\0';
			if (len > 0 && add_history(line) != 0)
				return (close(fd), 1);
			len = 0;
		}
		else
			line[len++] = c;
	}
	if (len > 0)
	{
		line[len] = '\0';
		if (add_history(line) != 0)
			return (close(fd), 1);
	}
	close(fd);
	return (count == -1);
}

/**
 * handle_history - prints the current history
 * @args: command arguments
 * @program_name: shell program name
 *
 * Return: 0 on success, 1 on invalid arguments
 */
int handle_history(char **args, char *program_name)
{
	history_t *node = history_head;

	if (args[1] != NULL)
	{
		fprintf(stderr, "%s: history: too many arguments\n", program_name);
		return (1);
	}
	while (node != NULL)
	{
		printf("%lu %s\n", node->number, node->command);
		node = node->next;
	}
	return (0);
}

/**
 * save_history - saves history without line numbers
 *
 * Return: 0 on success, 1 on failure
 */
int save_history(void)
{
	char *home = NULL, path[4096];
	size_t i = 0, length;
	history_t *node = history_head;
	int fd;

	while (environ[i] != NULL && home == NULL)
	{
		if (strncmp(environ[i], "HOME=", 5) == 0)
			home = environ[i] + 5;
		i++;
	}
	if (home == NULL ||
	    strlen(home) + strlen("/.simple_shell_history") >= sizeof(path))
		return (1);
	strcpy(path, home);
	strcat(path, "/.simple_shell_history");
	fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd == -1)
		return (1);
	while (node != NULL)
	{
		length = strlen(node->command);
		if (write(fd, node->command, length) != (ssize_t)length ||
		    write(fd, "\n", 1) != 1)
			return (close(fd), 1);
		node = node->next;
	}
	close(fd);
	return (0);
}

/**
 * free_history - frees all history entries
 */
void free_history(void)
{
	history_t *node = history_head;
	history_t *next;

	while (node != NULL)
	{
		next = node->next;
		free(node->command);
		free(node);
		node = next;
	}
	history_head = NULL;
	history_tail = NULL;
	history_next = 0;
}
