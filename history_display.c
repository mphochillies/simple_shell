#include "header.h"

/**
 * hist_list_disply - displays the history list, one command by line, preceded
 *              with line numbers, starting at 0.
 * @data: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */
int hist_list_disply(info_t *data)
{
	prnt_elements(data->history);
	return (0);
}

/**
 * alias_str_set - sets alias to string
 * @data: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int alias_str_set(info_t *data, char *str)
{
	char *p, c;
	int ret;

	p = find_str_char(str, '=');
	if (!p)
		return (1);
	c = *p;
	*p = 0;
	ret = del_node_idx(&(data->alias),
		index_node_get(data->alias, prefix_node_rtn(data->alias, str, -1)));
	*p = c;
	return (ret);
}

/**
 * set_alias_par - sets alias to string
 * @data: parameter struct
 * @str: the string alias
 *
 * Return: Always 0 on success, 1 on error
 */
int set_alias_par(info_t *data, char *str)
{
	char *p;

	p = find_str_char(str, '=');
	if (!p)
		return (1);
	if (!*++p)
		return (alias_str_set(data, str));

	alias_str_set(data, str);
	return (node_end(&(data->alias), str, 0) == NULL);
}

/**
 * display_alias_str - prints an alias string
 * @node: the alias node
 *
 * Return: Always 0 on success, 1 on error
 */
int display_alias_str(list_t *node)
{
	char *p = NULL, *a = NULL;

	if (node)
	{
		p = find_str_char(node->str, '=');
		for (a = node->str; a <= p; a++)
			stdout_write_char(*a);
		stdout_write_char('\'');
		_print_filescrpt(p + 1);
		_print_filescrpt("'\n");
		return (0);
	}
	return (1);
}

/**
 * builtin_mimic - mimics the alias builtin (man alias)
 * @data: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int builtin_mimic(info_t *data)
{
	int i = 0;
	char *p = NULL;
	list_t *node = NULL;

	if (data->argc == 1)
	{
		node = data->alias;
		while (node)
		{
			display_alias_str(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; data->argv[i]; i++)
	{
		p = find_str_char(data->argv[i], '=');
		if (p)
			set_alias_par(data, data->argv[i]);
		else
			display_alias_str(prefix_node_rtn(data->alias, data->argv[i], '='));
	}

	return (0);
}
