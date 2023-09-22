#include "header.h"

/**
 * environ_prnts - prints the current environment
 * @data: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int environ_prnts(info_t *data)
{
	str_element(data->env);
	return (0);
}

/**
 * env_var_get - gets the value of an environ variable
 * @data: Structure containing potential arguments. Used to maintain
 * @name: env var name
 *
 * Return: the value
 */
char *env_var_get(info_t *data, const char *name)
{
	list_t *node = data->env;
	char *p;

	while (node)
	{
		p = needle_start_check(node->str, name);
		if (p && *p)
			return (p);
		node = node->next;
	}
	return (NULL);
}

/**
 * new_env_initz - Initialize a new environment variable,
 *             or modify an existing one
 * @data: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int new_env_initz(info_t *data)
{
	if (data->argc != 3)
	{
		input_str_print("Incorrect number of arguements\n");
		return (1);
	}
	if (new_env_init(data, data->argv[1], data->argv[2]))
		return (0);
	return (1);
}

/**
 * remove_args_env - Remove an environment variable
 * @data: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int remove_args_env(info_t *data)
{
	int i;

	if (data->argc == 1)
	{
		input_str_print("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= data->argc; i++)
		del_env_var(data, data->argv[i]);

	return (0);
}

/**
 * link_env_popl - populates env linked list
 * @data: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
int link_env_popl(info_t *data)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		node_end(&node, environ[i], 0);
	data->env = node;
	return (0);
}

