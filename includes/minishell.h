/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:02:28 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/05 00:38:45 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libftprintf/inc/ft_printf.h"
# include "../libftprintf/inc/get_next_line.h"
# include "../libftprintf/inc/libft.h"
# include <curses.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/ioctl.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <unistd.h>

// --- GLOBAL VAR ---
extern int				g_signal_status;

// --- ENV LINKED LIST ---

typedef struct s_env
{
	char				*name;
	char				*value;
	struct s_env		*next;
}						t_env;

// --- TOKEN TYPES ---

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_AMP,
	TOKEN_EOF
}						t_token_type;

// --- WORD TYPES ---

typedef enum e_word_type
{
	WORD_UNQUOTED,
	WORD_SINGLE_QUOTED,
	WORD_DOUBLE_QUOTED
}						t_word_type;

// --- TOKEN LINKED LIST ---

typedef struct s_token
{
	char				*value;
	t_token_type		type;
	t_word_type			word_type;
	int					has_leading_space;
	struct s_token		*next;
}						t_token;

// --- COMMAND STRUCTURE ---

typedef enum e_operator
{
	OP_NONE,
	OP_PIPE,
	OP_AND,
	OP_OR
}						t_operator;

typedef struct s_redir
{
	char				*file;
	int					type;
	int					heredoc_expand;
	int					heredoc_fd;
	struct s_redir		*next;
}						t_redir;

typedef struct s_command
{
	char				**args;
	char				*input;
	char				*output;
	bool				append;
	int					fd[2];
	int					heredoc_expand;
	t_redir				*redir;
	t_operator			next_op;
	t_token_type		here_doc;
	char				*heredoc_delim;
	struct s_command	*next;
}						t_command;

// --- EXECUTION CONTEXT ---

typedef struct s_info
{
	pid_t				*pid;
	int					pipe_fd[2];
	int					in_fd;
	int					cmd_index;
	int					cmd_count;
	t_env				*env;
	t_command			*cmds;
	t_command			*first_cmds;
}						t_info;

// --- LEXER ---

t_token					*new_token(char *value, t_token_type type,
							t_word_type word_type, int has_leading_space);
void					add_token(t_token **tokens, char *value,
							t_token_type type, t_word_type word_type,
							int has_leading_space);
void					handle_token(char *input, int *i, t_token **tokens);
t_token					*lexer(char *input);
t_token_type			get_token_type(char *input, int *i);
void					extract_word(char *input, int *i, t_token **tokens);
t_token_type			get_token_type(char *input, int *i);

// --- PARSER ---

t_command				*new_command(void);
int						add_argument(t_command *cmd, char *arg,
							t_word_type word_type, t_env *env);
t_command				*handle_pipe(t_command *cmd);
t_command				*parse_tokens(t_token *tokens, t_env *env);
int						check_syntax_errors(t_token *tokens);
void					handle_redirection(t_command *cmd, t_token **tokens);
char					*expand_token_value(const char *str,
							t_word_type word_type, t_env *env, int last_status);
char					*expand_token_value_unquoted(const char *str,
							t_env *env, int last_status);
// --- UTILS TEST MAIN ---

void					print_tokens(t_token *tokens);
void					print_commands(t_command *cmds);

// --- TESTING EXEC ---
int						exec_command(t_command *cmd, t_env *env);
void					exec_child_builtin(t_command *cmds, t_info *info);
void					exec_child_external(t_command *cmds, t_info *info);
void					handle_child_process(t_command *cmds, t_info *info);

// --- BUILTINS ---
int						is_builtins(t_command *cmds);
void					run_builtins(t_command *cmds, t_info *info);
int						ft_echo(t_command *cmds);
int						ft_pwd(void);
void					ft_exit(char **args, t_info *info);
int						ft_env(t_env *env);
int						ft_export(t_env *env, char **args);
int						ft_unset(t_env **env, char *name);
int						ft_cd(t_command *cmds, t_env *env);

//--- PIPES ---
void					run_pipe(t_command *cmds, t_env *env);
int						command_redirections(t_command *cmd);
int						here_doc(const char *delim, int heredoc_expand,
							t_env *env);

//--- ENV ---
t_env					*new_env_node(char *name, char *value);
void					add_env_back(t_env **env, t_env *new);
t_env					*init_env(char **envp, t_env *env);
char					*ft_getenv(t_env *env, const char *name);
void					ft_setenv(t_env *env, const char *name,
							const char *value);
int						add_new_node(t_env *env, char *args);
//--- UTILS ---
void					free_command_list(t_command *cmd);
char					*build_full_path(char *dir, char *cmd);
int						find_cmd_in_path(t_command *cmds, t_env *env);
int						count_cmds(t_command *cmds);
void					close_fd(int *in_fd, t_command *cmds, int *pipe_fd);
int						env_found(t_env *env, char *name);
void					export_error(char *arg);
char					*find_plus_equal(char *str);
void					append_env_value(t_env *env, char *name,
							char *to_append);
int						handle_plus_equal(t_env *env, char *arg);
int						argv_parsed(char *name);
char					**env_to_envp(t_env *env);
char					**get_path_dirs(t_env *env);
void					free_env_list(t_env *env);
void					free_token_list(t_token *tokens);
int						validate_quotes(const char *input, int start,
							char quote);
void					sigint_handler(int sig);
void					restore_heredoc_signals(void);
int						prepare_heredocs(t_command *cmds, t_env *env);
void					child_process_signals(void);
void					close_all_heredoc_fds(t_command *cmds);
void					close_unused_heredoc_fds(t_command *current_cmd,
							t_command *all_cmds);
t_command				*init(t_token *tokens, t_command *cmds, char *input,
							t_env *env);
void					heredoc_sigint(int sig);
int						check_input(char *input);
void					setup_signals(void);
void					setup_heredoc_signals(void);
int						handle_redir_append(t_redir *redir);
int						handle_redir_out(t_redir *redir);
int						handle_redir_in(t_redir *redir);
t_redir					*find_last_heredoc(t_redir *redir);
int						redirect_input(char *infile);
int						redirect_output(char *outfile, bool append);
void					setup_child_fds(t_command *cmds, t_info *info);
void					wait_for_pids(t_command *cmds, pid_t *pid);
int						create_child_process(void);
int						create_pipe(t_command *cmds, int *pipe_fd);
int						is_parent_builtin(t_command *cmds);
void					free_info(t_info *info);
void					free_info_child(t_info *info);
t_info					*init_info(t_command *cmds, t_env *env);
void					launch_child_process(t_command *cmds, t_info *info);
void					exec_pipe(t_command *cmds, t_info *info);
int						handle_parent_builtin_if_needed(t_command *cmds,
							t_info *info);
void					free_info_and_exit(t_info *info, int exit_code);

#endif
