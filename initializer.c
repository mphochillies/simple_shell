#include "header.h"

/**
 * clear_info - initializes info_t struct
 * @data: struct address
 */
void clear_info(info_t *data)
{
	data->arg = NULL;
	data->argv = NULL;
	data->path = NULL;
	data->argc = 0;
}

/**
 * set_info - initializes info_t struct
 * @data: struct address
 * @av: argument vector
 */
void set_info(info_t *data, char **av)
{
	int i = 0;

	data->fname = av[0];
	if (data->arg)
	{
		data->argv = string_to_word1(data->arg, " \t");
		if (!data->argv)
		{

			data->argv = malloc(sizeof(char *) * 2);
			if (data->argv)
			{
				data->argv[0] = replicate_str(data->arg);
				data->argv[1] = NULL;
			}
		}
		for (i = 0; data->argv && data->argv[i]; i++)
			;
		data->argc = i;

		token_replace_al(data);
		token_var_replace(data);
	}
}

/**
 * free_info - frees info_t struct fields
 * @data: struct address
 * @all: true if freeing all fields
 */
void free_info(info_t *data, int all)
{
	release_string(data->argv);
	data->argv = NULL;
	data->path = NULL;
	if (all)
	{
		if (!data->cmd_buf)
			free(data->arg);
		if (data->env)
			del_node_list(&(data->env));
		if (data->history)
			del_node_list(&(data->history));
		if (data->alias)
			del_node_list(&(data->alias));
		release_string(data->environ);
			data->environ = NULL;
		null_addr((void **)data->cmd_buf);
		if (data->readfd > 2)
			close(data->readfd);
		stdout_write_char(BUF_FLUSH);
	}
}
