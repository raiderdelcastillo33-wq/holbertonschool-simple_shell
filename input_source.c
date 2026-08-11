#include "shell.h"
#include <fcntl.h>

/**
 * setup_input - selects the shell input source
 * @argc: command-line argument count
 * @argv: command-line argument vector
 * @fd: destination for the selected descriptor
 * @interactive: destination for interactive mode state
 *
 * Return: 0 on success, 1 on failure
 */
int setup_input(int argc, char **argv, int *fd, int *interactive)
{
	if (argc > 2)
	{
		write(STDERR_FILENO, "Usage: simple_shell [filename]\n", 31);
		return (1);
	}
	if (argc == 1)
	{
		*fd = STDIN_FILENO;
		*interactive = isatty(STDIN_FILENO);
		return (0);
	}
	*fd = open(argv[1], O_RDONLY);
	if (*fd == -1)
	{
		perror(argv[1]);
		return (1);
	}
	*interactive = 0;
	return (0);
}

/**
 * show_prompt - displays the shell prompt when interactive
 * @interactive: nonzero when prompt output is allowed
 */
void show_prompt(int interactive)
{
	if (interactive)
	{
		printf("#cisfun$ ");
		fflush(stdout);
	}
}

/**
 * close_input - closes a file input descriptor when necessary
 * @fd: active shell input descriptor
 */
void close_input(int fd)
{
	if (fd != STDIN_FILENO)
		close(fd);
}
