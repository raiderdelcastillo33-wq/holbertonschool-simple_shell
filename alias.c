#include "shell.h"

static alias_t *alias_list;

/**
 * find_alias - finds an alias by name
 * @name: alias name
 *
 * Return: matching alias, or NULL
 */
static alias_t *find_alias(const char *name)
{
	alias_t *node = alias_list;

	while (node != NULL)
	{
		if (strcmp(node->name, name) == 0)
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * set_alias_value - creates or replaces an alias
 * @name: alias name
 * @value: alias value
 *
 * Return: 0 on success, 1 on allocation failure
 */
int set_alias_value(const char *name, const char *value)
{
	alias_t *node, *tail;
	char *new_value;

	new_value = malloc(strlen(value) + 1);
	if (new_value == NULL)
		return (1);
	strcpy(new_value, value);
	node = find_alias(name);
	if (node != NULL)
	{
		free(node->value);
		node->value = new_value;
		return (0);
	}
	node = malloc(sizeof(*node));
	if (node == NULL)
	{
		free(new_value);
		return (1);
	}
	node->name = malloc(strlen(name) + 1);
	if (node->name == NULL)
	{
		free(new_value);
		free(node);
		return (1);
	}
	strcpy(node->name, name);
	node->value = new_value;
	node->next = NULL;
	if (alias_list == NULL)
		alias_list = node;
	else
	{
		tail = alias_list;
		while (tail->next != NULL)
			tail = tail->next;
		tail->next = node;
	}
	return (0);
}

/**
 * print_alias - prints one alias when it exists
 * @name: alias name
 */
void print_alias(const char *name)
{
	alias_t *node = find_alias(name);

	if (node != NULL)
		printf("%s='%s'\n", node->name, node->value);
}

/**
 * print_all_aliases - prints every stored alias
 */
void print_all_aliases(void)
{
	alias_t *node = alias_list;

	while (node != NULL)
	{
		printf("%s='%s'\n", node->name, node->value);
		node = node->next;
	}
}

/**
 * free_aliases - frees all shell aliases
 */
void free_aliases(void)
{
	alias_t *node = alias_list;
	alias_t *next;

	while (node != NULL)
	{
		next = node->next;
		free(node->name);
		free(node->value);
		free(node);
		node = next;
	}
	alias_list = NULL;
}
