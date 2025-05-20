/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:02:28 by tuthayak          #+#    #+#             */
/*   Updated: 2025/05/19 13:59:19 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
#define MINISHELL_H

#include "../libftprintf/inc/ft_printf.h"
#include "../libftprintf/inc/get_next_line.h"
#include "../libftprintf/inc/libft.h"
#include <curses.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <signal.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <term.h>
#include <termios.h>
#include <unistd.h>

// --- GLOBAL VAR ---
extern int g_signal_status;

// --- ENV LINKED LIST ---

typedef struct s_env
{
	char *name;
	char *value;
	struct s_env *next;
} t_env;

// --- TOKEN TYPES ---

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
} t_token_type;

// --- TOKEN LINKED LIST ---

typedef struct s_token
{
	char *value;
	t_token_type type;
	struct s_token *next;
} t_token;

// --- COMMAND STRUCTURE ---

typedef enum e_operator
{
	OP_NONE,
	OP_PIPE,
	OP_AND,
	OP_OR
} t_operator;

typedef struct s_redir
{
	char *file;
	int type;
	struct s_redir *next;
} t_redir;

typedef struct s_command
{
	char **args;
	char **path;
	char *input;
	char *output;
	bool append;
	int status;
	int fd[2];
	pid_t pid;
	t_redir *redir;
	t_operator next_op;
	t_token_type here_doc;
	struct s_command *next;
} t_command;

// --- LEXER ---

t_token *new_token(char *value, t_token_type type);
void add_token(t_token **tokens, char *value,
			   t_token_type type);
void handle_token(char *input, int *i, t_token **tokens);
t_token *lexer(char *input);
t_token_type get_token_type(char *input, int *i);
void extract_word(char *input, int *i, t_token **tokens);

// --- PARSER (examples) ---

t_command *new_command(void);
void add_argument(t_command *cmd, char *arg);
t_command *handle_pipe(t_command *cmd);
t_command *parse_tokens(t_token *tokens);
int check_syntax_errors(t_token *tokens);
void handle_redirection(t_command *cmd, t_token **tokens);

// --- UTILS TEST MAIN ---

void print_tokens(t_token *tokens);
void print_commands(t_command *cmds);

// --- TESTING EXEC ---
int exec_command(t_command *cmd, char **envp);
int init_command_path(t_command *cmd, char **envp);

// --- BUILTINS ---
int ft_echo(t_command *cmds);
int ft_pwd(void);
void ft_exit(char **args, t_command *cmds);
int free_struct(t_command *cmds);
char *build_full_path(char *dir, char *cmd);
void run_pipe(t_command *cmds, char **envp);
int find_cmd_in_path(t_command *cmds);
int is_builtins(t_command *cmds);
int command_redirections(t_command *cmd);
int here_doc(const char *delim);

#endif