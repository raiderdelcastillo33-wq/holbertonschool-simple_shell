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

/**
 * print_general_help - prints the general built-in help
 */
static void print_general_help(void)
{
	printf("Simple Shell built-ins:\n");
	printf("  exit [STATUS]\n");
	printf("  env\n");
	printf("  setenv VARIABLE VALUE\n");
	printf("  unsetenv VARIABLE\n");
	printf("  cd [DIRECTORY]\n");
	printf("  alias [name[='value'] ...]\n");
	printf("  help [BUILTIN]\n");
	printf("Use: help [BUILTIN]\n");
}

/**
 * print_builtin_help - prints help for one built-in
 * @name: built-in command name
 *
 * Return: 0 when help exists, 1 otherwise
 */
static int print_builtin_help(const char *name)
{
	static const char * const names[] = {
		"exit", "env", "setenv", "unsetenv",
		"cd", "alias", "help", NULL
	};
	static const char * const texts[] = {
		"exit [STATUS] - exit the shell",
		"env - print the current environment",
		"setenv VARIABLE VALUE - create or replace a variable",
		"unsetenv VARIABLE - remove an environment variable",
		"cd [DIRECTORY] - change the current directory",
		"alias [name[='value'] ...] - define or display aliases",
		"help [BUILTIN] - display built-in help"
	};
	int index;

	for (index = 0; names[index] != NULL; index++)
	{
		if (strcmp(name, names[index]) == 0)
		{
			printf("%s\n", texts[index]);
			return (0);
		}
	}
	return (1);
}

/**
 * handle_help - handles the help built-in command
 * @args: shell argument vector
 * @program_name: shell program name used in errors
 *
 * Return: 0 on success, 1 on invalid usage or unknown topic
 */
int handle_help(char **args, char *program_name)
{
	if (args[1] == NULL)
	{
		print_general_help();
		return (0);
	}
	if (args[2] != NULL)
	{
		fprintf(stderr, "%s: help: too many arguments\n", program_name);
		return (1);
	}
	if (print_builtin_help(args[1]) == 0)
		return (0);

	fprintf(stderr, "%s: help: no help for %s\n",
		program_name, args[1]);
	return (1);
}
