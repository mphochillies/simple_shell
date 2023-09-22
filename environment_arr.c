#include "header.h"

/**
 * return_str_arr - returns the string array copy of our environ
 * @data: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 * Return: Always 0
 */
char **return_str_arr(info_t *data)
{
	if (!data->environ || data->env_changed)
	{
		data->environ = return_array(data->env);
		data->env_changed = 0;
	}

	return (data->environ);
}

/**
 * del_env_var - Remove an environment variable
 * @data: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: 1 on delete, 0 otherwise
 * @var: the string env var property
 */
int del_env_var(info_t *data, char *var)
{
	list_t *node = data->env;
	size_t i = 0;
	char *p;

	if (!node || !var)
		return (0);

	while (node)
	{
		p = needle_start_check(node->str, var);
		if (p && *p == '=')
		{
			data->env_changed = del_node_idx(&(data->env), i);
			i = 0;
			node = data->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (data->env_changed);
}

/**
 * new_env_init - Initialize a new environment variable,
 *             or modify an existing one
 * @data: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *  Return: Always 0
 */
int new_env_init(info_t *data, char *var, char *value)
{
	char *buffer = NULL;
	list_t *node;
	char *p;

	if (!var || !value)
		return (0);

	buffer = malloc(return_len(var) + return_len(value) + 2);
	if (!buffer)
		return (1);
	copy_strg(buffer, var);
	join_two_str_dest(buffer, "=");
	join_two_str_dest(buffer, value);
	node = data->env;
	while (node)
	{
		p = needle_start_check(node->str, var);
		if (p && *p == '=')
		{
			free(node->str);
			node->str = buffer;
			data->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	node_end(&(data->env), buffer, 0);
	free(buffer);
	data->env_changed = 1;
	return (0);
}

