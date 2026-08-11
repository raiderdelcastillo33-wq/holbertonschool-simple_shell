#include "shell.h"

#define OP_NONE 0
#define OP_SEMICOLON 1
#define OP_AND 2
#define OP_OR 3

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
 * next_operator - finds the next command operator
 * @cursor: position from which to search
 * @operator_pos: storage for the operator position
 *
 * Return: operator type, or OP_NONE if no operator exists
 */
static int next_operator(char *cursor, char **operator_pos)
{
	while (*cursor != '\0')
	{
		if (*cursor == ';')
		{
			*operator_pos = cursor;
			return (OP_SEMICOLON);
		}
		if (*cursor == '&' && cursor[1] == '&')
		{
			*operator_pos = cursor;
			return (OP_AND);
		}
		if (*cursor == '|' && cursor[1] == '|')
		{
			*operator_pos = cursor;
			return (OP_OR);
		}
		cursor++;
	}
	*operator_pos = NULL;
	return (OP_NONE);
}

/**
 * should_execute - checks whether a logical segment must run
 * @operator: operator preceding the segment
 * @status: status of the last executed command
 *
 * Return: 1 if the segment must execute, 0 otherwise
 */
static int should_execute(int operator, int status)
{
	if (operator == OP_AND)
		return (status == 0);
	if (operator == OP_OR)
		return (status != 0);

	return (1);
}

/**
 * process_logical_chain - executes one logical command chain
 * @chain: chain containing logical operators
 * @program_name: shell program name
 * @last_status: status before the chain
 * @exit_status: status to return if exit is requested
 * @line_number: physical input line number used in error messages
 *
 * Return: status of the last executed command or SHELL_EXIT
 */
static int process_logical_chain(char *chain, char *program_name,
	int last_status, int *exit_status, int line_number)
{
	char *segment = chain, *cursor = chain, *operator_pos;
	int status = last_status, previous = OP_NONE, operator;

	while (1)
	{
		operator = next_operator(cursor, &operator_pos);
		if (operator != OP_NONE)
			*operator_pos = '\0';
		if (segment_has_command(segment) &&
		    should_execute(previous, status))
		{
			status = process_line(segment, program_name, status,
					      exit_status, line_number);
			if (status == SHELL_EXIT)
				return (SHELL_EXIT);
		}
		if (operator == OP_NONE || operator == OP_SEMICOLON)
			break;
		previous = operator;
		cursor = operator_pos + 2;
		segment = cursor;
	}
	return (status);
}

/**
 * process_sequence - executes command sequences and logical chains
 * @line: input line
 * @program_name: shell program name
 * @last_status: status before processing the sequence
 * @exit_status: status to return if exit is requested
 * @line_number: physical input line number used in error messages
 *
 * Return: status of the last executed command or SHELL_EXIT
 */
int process_sequence(char *line, char *program_name, int last_status,
		     int *exit_status, int line_number)
{
	char *segment = line, *cursor, *operator_pos;
	int status = last_status, operator;

	while (1)
	{
		cursor = segment;
		do {
			operator = next_operator(cursor, &operator_pos);
			if (operator == OP_AND || operator == OP_OR)
				cursor = operator_pos + 2;
		} while (operator == OP_AND || operator == OP_OR);
		if (operator == OP_SEMICOLON)
			*operator_pos = '\0';
		if (segment_has_command(segment))
		{
			status = process_logical_chain(segment, program_name,
						       status, exit_status,
						       line_number);
			if (status == SHELL_EXIT)
				return (SHELL_EXIT);
		}
		if (operator == OP_NONE)
			break;
		segment = operator_pos + 1;
	}
	return (status);
}
