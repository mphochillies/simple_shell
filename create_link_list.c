#include "header.h"

/**
 * len_link_check - determines length of linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t len_link_check(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * return_array - returns an array of strings of the list->str
 * @head: pointer to first node
 *
 * Return: array of strings
 */
char **return_array(list_t *head)
{
	list_t *node = head;
	size_t i = len_link_check(head), j;
	char **strs;
	char *str;

	if (!head || !i)
		return (NULL);
	strs = malloc(sizeof(char *) * (i + 1));
	if (!strs)
		return (NULL);
	for (i = 0; node; node = node->next, i++)
	{
		str = malloc(return_len(node->str) + 1);
		if (!str)
		{
			for (j = 0; j < i; j++)
				free(strs[j]);
			free(strs);
			return (NULL);
		}

		str = copy_strg(str, node->str);
		strs[i] = str;
	}
	strs[i] = NULL;
	return (strs);
}


/**
 * prnt_elements - prints all elements of a list_t linked list
 * @h: pointer to first node
 *
 * Return: size of list
 */
size_t prnt_elements(const list_t *h)
{
	size_t i = 0;

	while (h)
	{
		_print_filescrpt(clone_convt_func(h->num, 10, 0));
		stdout_write_char(':');
		stdout_write_char(' ');
		_print_filescrpt(h->str ? h->str : "(nil)");
		_print_filescrpt("\n");
		h = h->next;
		i++;
	}
	return (i);
}

/**
 * prefix_node_rtn - returns node whose string starts with prefix
 * @node: pointer to list head
 * @prefix: string to match
 * @c: the next character after prefix to match
 *
 * Return: match node or null
 */
list_t *prefix_node_rtn(list_t *node, char *prefix, char c)
{
	char *p = NULL;

	while (node)
	{
		p = needle_start_check(node->str, prefix);
		if (p && ((c == -1) || (*p == c)))
			return (node);
		node = node->next;
	}
	return (NULL);
}

/**
 * index_node_get - gets the index of a node
 * @head: pointer to list head
 * @node: pointer to the node
 *
 * Return: index of node or -1
 */
ssize_t index_node_get(list_t *head, list_t *node)
{
	size_t i = 0;

	while (head)
	{
		if (head == node)
			return (i);
		head = head->next;
		i++;
	}
	return (-1);
}

