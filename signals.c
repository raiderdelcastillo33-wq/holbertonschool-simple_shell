#include "shell.h"

/**
 * handle_sigint - handles Ctrl+C while waiting for input
 * @signal_number: received signal number
 */
static void handle_sigint(int signal_number)
{
	(void)signal_number;
	write(STDOUT_FILENO, "\n#cisfun$ ", 10);
}

/**
 * install_shell_sigint - installs the interactive SIGINT handler
 */
void install_shell_sigint(void)
{
	if (isatty(STDIN_FILENO))
		signal(SIGINT, handle_sigint);
}

/**
 * ignore_shell_sigint - ignores SIGINT in the interactive shell parent
 */
void ignore_shell_sigint(void)
{
	if (isatty(STDIN_FILENO))
		signal(SIGINT, SIG_IGN);
}
