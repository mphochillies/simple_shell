#include "header.h"

/**
 * buffer_chain - buffers chained commands
 * @data: parameter struct
 * @buffer: address of buffer
 * @len: address of len var
 *
 * Return: bytes read
 */
ssize_t buffer_chain(info_t *data, char **buffer, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len) /* if nothing left in the buffer, fill it */
	{
		/*null_addr((void **)data->cmd_buf);*/
		free(*buffer);
		*buffer = NULL;
		signal(SIGINT, blockattr);
#if USEget_stdn_line
		r = getline(buffer, &len_p, stdin);
#else
		r = get_stdn_line(data, buffer, &len_p);
#endif
		if (r > 0)
		{
			if ((*buffer)[r - 1] == '\n')
			{
				(*buffer)[r - 1] = '\0'; /* remove trailing newline */
				r--;
			}
			data->linecount_flag = 1;
			inst_replacer(*buffer);
			hist_link_list_add(data, *buffer, data->histcount++);
			/* if (find_str_char(*buffer, ';')) is this a command chain? */
			{
				*len = r;
				data->cmd_buf = buffer;
			}
		}
	}
	return (r);
}

/**
 * get_new_line - gets a line minus the newline
 * @data: parameter struct
 *
 * Return: bytes read
 */
ssize_t get_new_line(info_t *data)
{
	static char *buffer; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(data->arg), *p;

	stdout_write_char(BUF_FLUSH);
	r = buffer_chain(data, &buffer, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* we have commands left in the chain buffer */
	{
		j = i; /* init new iterator to current buffer position */
		p = buffer + i; /* get pointer for return */

		base_chain_st(data, buffer, &j, i, len);
		while (j < len) /* iterate to semicolon or end */
		{
			if (test_curr_char(data, buffer, &j))
				break;
			j++;
		}

		i = j + 1; /* increment past nulled ';'' */
		if (i >= len) /* reached end of buffer? */
		{
			i = len = 0; /* reset position and length */
			data->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* pass back pointer to current command position */
		return (return_len(p)); /* return length of current command */
	}

	*buf_p = buffer; /* else not a chain, pass back buffer from get_stdn_line() */
	return (r); /* return length of buffer from get_stdn_line() */
}

/**
 * get_buffer_r - reads a buffer
 * @data: parameter struct
 * @buffer: buffer
 * @i: size
 *
 * Return: r
 */
ssize_t get_buffer_r(info_t *data, char *buffer, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(data->readfd, buffer, get_buffer_r_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * get_stdn_line - gets the next line of input from STDIN
 * @data: parameter struct
 * @ptr: address of pointer to buffer, preallocated or NULL
 * @length: size of preallocated ptr buffer if not NULL
 *
 * Return: s
 */
int get_stdn_line(info_t *data, char **ptr, size_t *length)
{
	static char buffer[get_buffer_r_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = get_buffer_r(data, buffer, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = find_str_char(buffer + i, '\n');
	k = c ? 1 + (unsigned int)(c - buffer) : len;
	new_p = mem_alloc_blk(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		join_str(new_p, buffer + i, k - i);
	else
		cpy_str(new_p, buffer + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * blockattr - blocks ctrl-C
 * @sig_num: the signal number
 *
 * Return: void
 */
void blockattr(__attribute__((unused))int sig_num)
{
	_print_filescrpt("\n");
	_print_filescrpt("$ ");
	stdout_write_char(BUF_FLUSH);
}

