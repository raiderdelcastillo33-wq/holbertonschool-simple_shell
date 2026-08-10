#include "shell.h"

#define INPUT_BUFFER_SIZE 1024
#define LINE_INITIAL_SIZE 128

/**
 * append_character - appends one character to a dynamic line
 * @line: line being built
 * @length: current line length
 * @capacity: allocated capacity
 * @character: character to append
 *
 * Return: 0 on success, -1 on failure
 */
static int append_character(char **line, size_t *length,
	size_t *capacity, char character)
{
	char *new_line;

	if (*length + 1 >= *capacity)
	{
		*capacity *= 2;
		new_line = realloc(*line, *capacity);
		if (new_line == NULL)
			return (-1);
		*line = new_line;
	}

	(*line)[*length] = character;
	(*length)++;
	return (0);
}

/**
 * refill_buffer - reads a block from standard input
 * @buffer: input buffer
 * @position: current buffer position
 * @available: valid bytes in the buffer
 *
 * Return: number of bytes read, 0 on EOF, or -1 on error
 */
static ssize_t refill_buffer(char *buffer, size_t *position,
	size_t *available)
{
	ssize_t count;

	count = read(STDIN_FILENO, buffer, INPUT_BUFFER_SIZE);
	if (count > 0)
	{
		*position = 0;
		*available = (size_t)count;
	}

	return (count);
}

/**
 * finish_line - terminates a completed input line
 * @line: destination pointer
 * @result: allocated line
 * @length: line length
 *
 * Return: line length, or 0 when no data remains
 */
static ssize_t finish_line(char **line, char *result, size_t length)
{
	if (length == 0)
	{
		free(result);
		return (0);
	}

	result[length] = '\0';
	*line = result;
	return ((ssize_t)length);
}

/**
 * shell_getline - reads one logical line from standard input
 * @line: destination pointer for the allocated line
 *
 * Return: line length, 0 on EOF, or -1 on failure
 */
ssize_t shell_getline(char **line)
{
	static char buffer[INPUT_BUFFER_SIZE];
	static size_t position;
	static size_t available;
	char *result;
	size_t length = 0;
	size_t capacity = LINE_INITIAL_SIZE;
	ssize_t count;
	char current;

	if (line == NULL)
		return (-1);
	*line = NULL;
	result = malloc(capacity);
	if (result == NULL)
		return (-1);
	while (1)
	{
		if (position >= available)
		{
			count = refill_buffer(buffer, &position, &available);
			if (count == -1)
			{
				free(result);
				return (-1);
			}
			if (count == 0)
				return (finish_line(line, result, length));
		}
		current = buffer[position++];

		if (append_character(&result, &length,
				     &capacity, current) == -1)
		{
			free(result);
			return (-1);
		}

		if (current == '\n')
			return (finish_line(line, result, length));
	}
}
