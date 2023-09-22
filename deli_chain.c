#include "header.h"

/**
 * test_curr_char - test if current char in buffer is a chain delimeter
 * @data: the parameter struct
 * @buffer: the char buffer
 * @p: address of current position in buffer
 *
 * Return: 1 if chain delimeter, 0 otherwise
 */
int test_curr_char(info_t *data, char *buffer, size_t *p)
{
	size_t j = *p;

	if (buffer[j] == '|' && buffer[j + 1] == '|')
	{
		buffer[j] = 0;
		j++;
		data->cmd_buf_type = CMD_OR;
	}
	else if (buffer[j] == '&' && buffer[j + 1] == '&')
	{
		buffer[j] = 0;
		j++;
		data->cmd_buf_type = CMD_AND;
	}
	else if (buffer[j] == ';') /* found end of this command */
	{
		buffer[j] = 0; /* replace semicolon with null */
		data->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*p = j;
	return (1);
}

/**
 * base_chain_st - checks we should continue chaining based on last status
 * @data: the parameter struct
 * @buffer: the char buffer
 * @p: address of current position in buffer
 * @i: starting position in buffer
 * @len: length of buffer
 *
 * Return: Void
 */
void base_chain_st(info_t *data, char *buffer, size_t *p, size_t i, size_t len)
{
	size_t j = *p;

	if (data->cmd_buf_type == CMD_AND)
	{
		if (data->status)
		{
			buffer[i] = 0;
			j = len;
		}
	}
	if (data->cmd_buf_type == CMD_OR)
	{
		if (!data->status)
		{
			buffer[i] = 0;
			j = len;
		}
	}

	*p = j;
}

/**
 * token_replace_al - replaces an aliases in the tokenized string
 * @data: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int token_replace_al(info_t *data)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = prefix_node_rtn(data->alias, data->argv[0], '=');
		if (!node)
			return (0);
		free(data->argv[0]);
		p = find_str_char(node->str, '=');
		if (!p)
			return (0);
		p = replicate_str(p + 1);
		if (!p)
			return (0);
		data->argv[0] = p;
	}
	return (1);
}

/**
 * token_var_replace - replaces vars in the tokenized string
 * @data: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int token_var_replace(info_t *data)
{
	int i = 0;
	list_t *node;

	for (i = 0; data->argv[i]; i++)
	{
		if (data->argv[i][0] != '$' || !data->argv[i][1])
			continue;

		if (!compare_two_strn(data->argv[i], "$?"))
		{
			new_str_replace(&(data->argv[i]),
				replicate_str(clone_convt_func(data->status, 10, 0)));
			continue;
		}
		if (!compare_two_strn(data->argv[i], "$$"))
		{
			new_str_replace(&(data->argv[i]),
				replicate_str(clone_convt_func(getpid(), 10, 0)));
			continue;
		}
		node = prefix_node_rtn(data->env, &data->argv[i][1], '=');
		if (node)
		{
			new_str_replace(&(data->argv[i]),
				replicate_str(find_str_char(node->str, '=') + 1));
			continue;
		}
		new_str_replace(&data->argv[i], replicate_str(""));

	}
	return (0);
}

/**
 * new_str_replace - replaces string
 * @old: address of old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int new_str_replace(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}

