#include "header.h"

/**
 * gen_hist_files - gets the history file
 * @data: parameter struct
 *
 * Return: allocated string containg history file
 */

char *gen_hist_files(info_t *data)
{
	char *buffer, *dir;

	dir = env_var_get(data, "HOME=");
	if (!dir)
		return (NULL);
	buffer = malloc(sizeof(char) * (return_len(dir) + return_len(HIST_FILE) + 2));
	if (!buffer)
		return (NULL);
	buffer[0] = 0;
	copy_strg(buffer, dir);
	join_two_str_dest(buffer, "/");
	join_two_str_dest(buffer, HIST_FILE);
	return (buffer);
}

/**
 * app_hist_files - creates a file, or appends to an existing file
 * @data: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int app_hist_files(info_t *data)
{
	ssize_t fd;
	char *filename = gen_hist_files(data);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = data->history; node; node = node->next)
	{
		filescrpt_printer(node->str, fd);
		filescrpt_wrter('\n', fd);
	}
	filescrpt_wrter(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * hist_frm_file - reads history from file
 * @data: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int hist_frm_file(info_t *data)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buffer = NULL, *filename = gen_hist_files(data);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buffer = malloc(sizeof(char) * (fsize + 1));
	if (!buffer)
		return (0);
	rdlen = read(fd, buffer, fsize);
	buffer[fsize] = 0;
	if (rdlen <= 0)
		return (free(buffer), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buffer[i] == '\n')
		{
			buffer[i] = 0;
			hist_link_list_add(data, buffer + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		hist_link_list_add(data, buffer + last, linecount++);
	free(buffer);
	data->histcount = linecount;
	while (data->histcount-- >= HIST_MAX)
		del_node_idx(&(data->history), 0);
	regen_hist_files(data);
	return (data->histcount);
}

/**
 * hist_link_list_add - adds entry to a history linked list
 * @data: Structure containing potential arguments. Used to maintain
 * @buffer: buffer
 * @linecount: the history linecount, histcount
 *
 * Return: Always 0
 */
int hist_link_list_add(info_t *data, char *buffer, int linecount)
{
	list_t *node = NULL;

	if (data->history)
		node = data->history;
	node_end(&node, buffer, linecount);

	if (!data->history)
		data->history = node;
	return (0);
}

/**
 * regen_hist_files - renumbers the history linked list after changes
 * @data: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int regen_hist_files(info_t *data)
{
	list_t *node = data->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (data->histcount = i);
}
