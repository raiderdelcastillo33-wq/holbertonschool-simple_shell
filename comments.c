#include "shell.h"

/**
 * strip_comment - removes a comment from a command line
 * @line: command line to inspect
 *
 * Description: replaces the first '#' and everything after it
 * logically by terminating the string at that position.
 */
void strip_comment(char *line)
{
	while (*line != '\0')
	{
		if (*line == '#')
		{
			*line = '\0';
			return;
		}
		line++;
	}
}
