#include "shell.h"

/**
 * cleanup_shell - releases shell resources before termination
 * @line: current input line to release
 */
void cleanup_shell(char *line)
{
	free(line);
	save_history();
	free_history();
	free_aliases();
	free_shell_env();
}
