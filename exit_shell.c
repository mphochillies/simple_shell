#include "header.h"

/**
 * logout_sh - exits the shell
 * @data: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: exits with a given exit status
 *         (0) if data.argv[0] != "exit"
 */
int logout_sh(info_t *data)
{
	int exitcheck;

	if (data->argv[1])  /* If there is an exit arguement */
	{
		exitcheck = convt_strn_int(data->argv[1]);
		if (exitcheck == -1)
		{
			data->status = 2;
			err_msg_handler(data, "Illegal number: ");
			input_str_print(data->argv[1]);
			char_print_c('\n');
			return (1);
		}
		data->err_num = convt_strn_int(data->argv[1]);
		return (-2);
	}
	data->err_num = -1;
	return (-2);
}

/**
 * change_curr_dir - changes the current directory of the process
 * @data: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int change_curr_dir(info_t *data)
{
	char *s, *dir, buffer[1024];
	int chdir_ret;

	s = getcwd(buffer, 1024);
	if (!s)
		_print_filescrpt("TODO: >>getcwd failure emsg here<<\n");
	if (!data->argv[1])
	{
		dir = env_var_get(data, "HOME=");
		if (!dir)
			chdir_ret = /* TODO: what should this be? */
				chdir((dir = env_var_get(data, "PWD=")) ? dir : "/");
		else
			chdir_ret = chdir(dir);
	}
	else if (compare_two_strn(data->argv[1], "-") == 0)
	{
		if (!env_var_get(data, "OLDPWD="))
		{
			_print_filescrpt(s);
			stdout_write_char('\n');
			return (1);
		}
		_print_filescrpt(env_var_get(data, "OLDPWD=")), stdout_write_char('\n');
		chdir_ret = /* TODO: what should this be? */
			chdir((dir = env_var_get(data, "OLDPWD=")) ? dir : "/");
	}
	else
		chdir_ret = chdir(data->argv[1]);
	if (chdir_ret == -1)
	{
		err_msg_handler(data, "can't cd to ");
		input_str_print(data->argv[1]), char_print_c('\n');
	}
	else
	{
		new_env_init(data, "OLDPWD", env_var_get(data, "PWD="));
		new_env_init(data, "PWD", getcwd(buffer, 1024));
	}
	return (0);
}

/**
 * process_curr_dir - changes the current directory of the process
 * @data: Structure containing potential arguments. Used to maintain
 *          constant function prototype.
 *  Return: Always 0
 */
int process_curr_dir(info_t *data)
{
	char **arg_array;

	arg_array = data->argv;
	_print_filescrpt("help call works. Function not yet implemented \n");
	if (0)
		_print_filescrpt(*arg_array); /* temp att_unused workaround */
	return (0);
}

