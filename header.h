#ifndef _HEADER_H_
#define _HEADER_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>


#define get_buffer_r_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1


#define CMD_NORM	0
#define CMD_OR		1
#define CMD_AND		2
#define CMD_CHAIN	3

#define CONVERT_LOWERCASE	1
#define CONVERT_UNSIGNED	2


#define USEget_stdn_line 0
#define USE_STRTOK 0

#define HIST_FILE	".simple_shell_history"
#define HIST_MAX	4096

extern char **environ;


/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
	int num;
	char *str;
	struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguements to pass into a function,
 *		allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguements
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
	char *arg;
	char **argv;
	char *path;
	int argc;
	unsigned int line_count;
	int err_num;
	int linecount_flag;
	char *fname;
	list_t *env;
	list_t *history;
	list_t *alias;
	char **environ;
	int env_changed;
	int status;

	char **cmd_buf; /* pointer to cmd ; chain buffer, for memory mangement */
	int cmd_buf_type; /* CMD_type ||, &&, ; */
	int readfd;
	int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
	0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
	char *type;
	int (*func)(info_t *);
} builtin_table;



int hsh(info_t *, char **);
int builtin_lookup(info_t *);
void command_find(info_t *);
void run_thread(info_t *);


int cmd_exec_file(info_t *, char *);
char *dup_character(char *, int, int);
char *cmd_find_path(info_t *, char *, char *);


int loophsh(char **);


void input_str_print(char *);
int char_print_c(char);
int filescrpt_wrter(char c, int fd);
int filescrpt_printer(char *str, int fd);


int return_len(char *);
int compare_two_strn(char *, char *);
char *needle_start_check(const char *, const char *);
char *join_two_str_dest(char *, char *);


char *copy_strg(char *, char *);
char *replicate_str(const char *);
void _print_filescrpt(char *);
int stdout_write_char(char);


char *cpy_str(char *, char *, int);
char *join_str(char *, char *, int);
char *find_str_char(char *, char);


char **string_to_word1(char *, char *);
char **string_to_word2(char *, char);


char *const_mem_fill(char *, char, unsigned int);
void release_string(char **);
void *mem_alloc_blk(void *, unsigned int, unsigned int);


int null_addr(void **);


int Inter_shell_mod(info_t *);
int char_delim(char, char *);
int _apha_char(int);
int convt_strr_int1(char *);

int convt_strn_int(char *);
void err_msg_handler(info_t *, char *);
int base_printer(int, int);
char *clone_convt_func(long int, int, int);
void inst_replacer(char *);


int logout_sh(info_t *);
int change_curr_dir(info_t *);
int process_curr_dir(info_t *);


int hist_list_disply(info_t *);
int builtin_mimic(info_t *);


ssize_t get_new_line(info_t *);
int get_stdn_line(info_t *, char **, size_t *);
void blockattr(int);


void clear_info(info_t *);
void set_info(info_t *, char **);
void free_info(info_t *, int);


char *env_var_get(info_t *, const char *);
int environ_prnts(info_t *);
int new_env_initz(info_t *);
int remove_args_env(info_t *);
int link_env_popl(info_t *);


char **return_str_arr(info_t *);
int del_env_var(info_t *, char *);
int new_env_init(info_t *, char *, char *);


char *gen_hist_files(info_t *data);
int app_hist_files(info_t *data);
int hist_frm_file(info_t *data);
int hist_link_list_add(info_t *data, char *buffer, int linecount);
int regen_hist_files(info_t *data);


list_t *node_point_add(list_t **, const char *, int);
list_t *node_end(list_t **, const char *, int);
size_t str_element(const list_t *);
int del_node_idx(list_t **, unsigned int);
void del_node_list(list_t **);


size_t len_link_check(const list_t *);
char **return_array(list_t *);
size_t prnt_elements(const list_t *);
list_t *prefix_node_rtn(list_t *, char *, char);
ssize_t index_node_get(list_t *, list_t *);


int test_curr_char(info_t *, char *, size_t *);
void base_chain_st(info_t *, char *, size_t *, size_t, size_t);
int token_replace_al(info_t *);
int token_var_replace(info_t *);
int new_str_replace(char **, char *);

#endif
