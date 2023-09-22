#include "header.h"

/**
 * hsh - main shell loop
 * @data: the parameter & return data struct
 * @av: the argument vector from main()
 *
 * Return: 0 on success, 1 on error, or error code
 */
int hsh(info_t *data, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(data);
		if (Inter_shell_mod(data))
			_print_filescrpt("$ ");
		char_print_c(BUF_FLUSH);
		r = get_new_line(data);
		if (r != -1)
		{
			set_info(data, av);
			builtin_ret = builtin_lookup(data);
			if (builtin_ret == -1)
				command_find(data);
		}
		else if (Inter_shell_mod(data))
			stdout_write_char('\n');
		free_info(data, 0);
	}
	app_hist_files(data);
	free_info(data, 1);
	if (!Inter_shell_mod(data) && data->status)
		exit(data->status);
	if (builtin_ret == -2)
	{
		if (data->err_num == -1)
			exit(data->status);
		exit(data->err_num);
	}
	return (builtin_ret);
}

/**
 * builtin_lookup - finds a builtin command
 * @data: the parameter & return data struct
 *
 * Return: -1 if builtin not found,
 *			0 if builtin executed successfully,
 *			1 if builtin found but not successful,
 *			-2 if builtin signals exit()
 */
int builtin_lookup(info_t *data)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", logout_sh},
		{"env", environ_prnts},
		{"help", process_curr_dir},
		{"history", hist_list_disply},
		{"setenv", new_env_initz},
		{"unsetenv", remove_args_env},
		{"cd", change_curr_dir},
		{"alias", builtin_mimic},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (compare_two_strn(data->argv[0], builtintbl[i].type) == 0)
		{
			data->line_count++;
			built_in_ret = builtintbl[i].func(data);
			break;
		}
	return (built_in_ret);
}

/**
 * command_find - finds a command in PATH
 * @data: the parameter & return data struct
 *
 * Return: void
 */
void command_find(info_t *data)
{
	char *path = NULL;
	int i, k;

	data->path = data->argv[0];
	if (data->linecount_flag == 1)
	{
		data->line_count++;
		data->linecount_flag = 0;
	}
	for (i = 0, k = 0; data->arg[i]; i++)
		if (!char_delim(data->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = cmd_find_path(data, env_var_get(data, "PATH="), data->argv[0]);
	if (path)
	{
		data->path = path;
		run_thread(data);
	}
	else
	{
		if ((Inter_shell_mod(data) || env_var_get(data, "PATH=")
			|| data->argv[0][0] == '/') && cmd_exec_file(data, data->argv[0]))
			run_thread(data);
		else if (*(data->arg) != '\n')
		{
			data->status = 127;
			err_msg_handler(data, "not found\n");
		}
	}
}

/**
 * run_thread - forks an exec thread to run cmd
 * @data: the parameter & return data struct
 *
 * Return: void
 */
void run_thread(info_t *data)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(data->path, data->argv, return_str_arr(data)) == -1)
		{
			free_info(data, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(data->status));
		if (WIFEXITED(data->status))
		{
			data->status = WEXITSTATUS(data->status);
			if (data->status == 126)
				err_msg_handler(data, "Permission denied\n");
		}
	}
}
