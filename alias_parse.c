#include "shell.h"

/**
 * skip_alias_spaces - skips spaces and tabs
 * @cursor: current parser position
 */
static void skip_alias_spaces(char **cursor)
{
	while (**cursor == ' ' || **cursor == '\t')
		(*cursor)++;
}

/**
 * read_alias_value - isolates an alias value
 * @cursor: current parser position
 *
 * Return: pointer to the isolated value
 */
static char *read_alias_value(char **cursor)
{
	char *value;

	if (**cursor == '\'')
	{
		(*cursor)++;
		value = *cursor;
		while (**cursor != '\0' && **cursor != '\'')
			(*cursor)++;
	}
	else
	{
		value = *cursor;
		while (**cursor != '\0' && **cursor != ' ' &&
		       **cursor != '\t')
			(*cursor)++;
	}
	if (**cursor != '\0')
		*(*cursor)++ = '\0';
	return (value);
}

/**
 * process_alias_entry - processes one alias argument
 * @cursor: current parser position
 *
 * Return: 0 on success, 1 on allocation failure
 */
static int process_alias_entry(char **cursor)
{
	char *name;
	char *value;

	skip_alias_spaces(cursor);
	if (**cursor == '\0')
		return (0);
	name = *cursor;
	while (**cursor != '\0' && **cursor != ' ' &&
	       **cursor != '\t' && **cursor != '=')
		(*cursor)++;
	if (**cursor == '=')
	{
		**cursor = '\0';
		(*cursor)++;
		value = read_alias_value(cursor);
		return (set_alias_value(name, value));
	}
	if (**cursor != '\0')
		*(*cursor)++ = '\0';
	print_alias(name);
	return (0);
}

/**
 * handle_alias_line - handles the alias built-in
 * @line: raw alias command line
 *
 * Return: 0 on success, 1 on allocation failure
 */
int handle_alias_line(char *line)
{
	char *cursor = line + 5;

	skip_alias_spaces(&cursor);
	if (*cursor == '\0')
	{
		print_all_aliases();
		return (0);
	}
	while (*cursor != '\0')
	{
		if (process_alias_entry(&cursor) != 0)
			return (1);
		skip_alias_spaces(&cursor);
	}
	return (0);
}
