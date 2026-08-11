#include "shell.h"

/**
 * segment_has_command - checks whether a segment contains a command
 * @segment: command segment
 *
 * Return: 1 if the segment contains non-blank characters, 0 otherwise
 */
static int segment_has_command(char *segment)
{
	while (*segment == ' ' || *segment == '\t')
		segment++;

	return (*segment != '\0');
}

/**
 * process_sequence - executes commands separated by semicolons
 * @line: input line
 * @program_name: shell program name
 * @last_status: status before processing the sequence
 * @exit_status: status to return if exit is requested
 *
 * Return: status of the last executed command or SHELL_EXIT
 */
int process_sequence(char *line, char *program_name, int last_status,
		     int *exit_status)
{
	char *segment = line;
	char *cursor = line;
	int status = last_status;
	int has_separator;

	while (1)
	{
		while (*cursor != '\0' && *cursor != ';')
			cursor++;
		has_separator = (*cursor == ';');
		if (has_separator)
			*cursor = '\0';
		if (segment_has_command(segment))
		{
			status = process_line(segment, program_name, status,
					      exit_status);
			if (status == SHELL_EXIT)
				return (SHELL_EXIT);
		}
		if (!has_separator)
			break;
		cursor++;
		segment = cursor;
	}
	return (status);
}
