#include "shell.h"

/**
 * get_env_value - gets the value of an environment variable
 * @name: variable name
 *
 * Return: pointer to value, or NULL if not found
 */
static char *get_env_value(const char *name)
{
	size_t length;
	int index;

	length = strlen(name);
	for (index = 0; environ[index] != NULL; index++)
	{
		if (strncmp(environ[index], name, length) == 0 &&
		    environ[index][length] == '=')
			return (environ[index] + length + 1);
	}
	return (NULL);
}

/**
 * get_current_directory - gets an allocated current directory path
 *
 * Return: allocated path, or NULL on failure
 */
static char *get_current_directory(void)
{
	return (getcwd(NULL, 0));
}

/**
 * resolve_cd_target - resolves the destination requested by cd
 * @args: command arguments
 * @print_path: receives 1 when cd - must print its destination
 *
 * Return: destination path, or NULL if unavailable
 */
static char *resolve_cd_target(char **args, int *print_path)
{
	*print_path = 0;
	if (args[1] == NULL)
		return (get_env_value("HOME"));
	if (args[2] != NULL)
		return (NULL);
	if (strcmp(args[1], "-") == 0)
	{
		*print_path = 1;
		return (get_env_value("OLDPWD"));
	}
	return (args[1]);
}

/**
 * update_cd_environment - updates OLDPWD and PWD
 * @oldcwd: directory before cd
 * @newcwd: directory after cd
 * @program_name: shell program name
 *
 * Return: 0 on success, 1 on failure
 */
static int update_cd_environment(char *oldcwd, char *newcwd,
				 char *program_name)
{
	char *old_args[4];
	char *new_args[4];

	old_args[0] = "setenv";
	old_args[1] = "OLDPWD";
	old_args[2] = oldcwd;
	old_args[3] = NULL;
	new_args[0] = "setenv";
	new_args[1] = "PWD";
	new_args[2] = newcwd;
	new_args[3] = NULL;
	if (handle_setenv(old_args, program_name) != 0)
		return (1);
	if (handle_setenv(new_args, program_name) != 0)
		return (1);
	return (0);
}

/**
 * handle_cd - changes the shell current working directory
 * @args: command arguments
 * @program_name: shell program name
 *
 * Return: 0 on success, 1 on failure
 */
int handle_cd(char **args, char *program_name)
{
	char *target, *oldcwd, *newcwd;
	int print_path;

	if (args[1] != NULL && args[2] != NULL)
		return (fprintf(stderr, "%s: cd: too many arguments\n",
			       program_name), 1);
	target = resolve_cd_target(args, &print_path);
	if (target == NULL || target[0] == '\0')
		return (fprintf(stderr, "%s: cd: missing destination\n",
			       program_name), 1);
	oldcwd = get_current_directory();
	if (oldcwd == NULL)
		return (perror(program_name), 1);
	if (chdir(target) == -1)
		return (free(oldcwd), perror(target), 1);
	newcwd = get_current_directory();
	if (newcwd == NULL)
		return (free(oldcwd), perror(program_name), 1);
	if (update_cd_environment(oldcwd, newcwd, program_name) != 0)
		return (free(oldcwd), free(newcwd), 1);
	if (print_path)
		printf("%s\n", newcwd);
	free(oldcwd);
	free(newcwd);
	return (0);
}
