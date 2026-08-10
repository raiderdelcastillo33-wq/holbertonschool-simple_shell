#include "shell.h"
#include <limits.h>

/**
 * parse_exit_value - converts an exit argument to an 8-bit status
 * @text: numeric argument
 * @status: resulting shell exit status
 *
 * Return: 1 on success, 0 on invalid input or overflow
 */
static int parse_exit_value(const char *text, int *status)
{
	unsigned long value = 0, limit;
	int negative = 0, digit;

	if (*text == '+' || *text == '-')
	{
		negative = (*text == '-');
		text++;
	}
	if (*text == '\0')
		return (0);

	limit = negative ? (unsigned long)LONG_MAX + 1UL :
		(unsigned long)LONG_MAX;

	while (*text != '\0')
	{
		if (*text < '0' || *text > '9')
			return (0);
		digit = *text - '0';
		if (value > (limit - (unsigned long)digit) / 10UL)
			return (0);
		value = value * 10UL + (unsigned long)digit;
		text++;
	}

	value %= 256UL;
	*status = negative ? (int)((256UL - value) % 256UL) : (int)value;
	return (1);
}

/**
 * handle_exit - handles the exit built-in command
 * @args: shell argument vector
 * @program_name: shell program name used in errors
 * @last_status: status of the previous command
 * @exit_status: status the shell must return when exiting
 *
 * Return: SHELL_EXIT when exiting, otherwise command status
 */
int handle_exit(char **args, char *program_name,
	int last_status, int *exit_status)
{
	if (args[1] == NULL)
	{
		*exit_status = last_status;
		return (SHELL_EXIT);
	}
	if (args[2] != NULL)
	{
		fprintf(stderr, "%s: exit: too many arguments\n", program_name);
		return (1);
	}
	if (!parse_exit_value(args[1], exit_status))
	{
		fprintf(stderr, "%s: exit: %s: numeric argument required\n",
			program_name, args[1]);
		*exit_status = 255;
	}
	return (SHELL_EXIT);
}
