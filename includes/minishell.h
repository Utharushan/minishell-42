/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 16:02:28 by tuthayak          #+#    #+#             */
/*   Updated: 2025/04/29 18:05:01 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdbool.h>
# include "../libftprintf/inc/libft.h"
# include "../libftprintf/inc/ft_printf.h"
# include "../libftprintf/inc/get_next_line.h"

// --- ENV LINKED LIST ---

typedef struct s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

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
}	t_token_type;

// --- TOKEN LINKED LIST ---

typedef struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

// --- COMMAND STRUCTURE ---

typedef enum e_operator
{
	OP_NONE,
	OP_PIPE,
	OP_AND,
	OP_OR
}	t_operator;

typedef struct s_command
{
	char				**args;
	char				**path;
	char				*input;
	char				*output;
	bool				append;
	t_operator			next_op;
	struct s_command	*next;
}	t_command;

// --- LEXER ---

t_token				*new_token(char *value, t_token_type type);
void				add_token(t_token **tokens, char *value, t_token_type type);
void				handle_token(char *input, int *i, t_token **tokens);
t_token				*lexer(char *input);
t_token_type		get_token_type(char *input, int *i);
void				extract_word(char *input, int *i, t_token **tokens);

// --- PARSER (examples) ---

t_command			*new_command(void);
void				add_argument(t_command *cmd, char *arg);
t_command			*handle_pipe(t_command *cmd);
t_command			*parse_tokens(t_token *tokens);
int					check_syntax_errors(t_token *tokens);
void				handle_redirection(t_command *cmd, t_token **tokens);

// --- UTILS TEST MAIN ---

void				print_tokens(t_token *tokens);
void				print_commands(t_command *cmds);

// --- TESTING EXEC ---
int					exec_command(t_command *cmd, char **envp);
int					command_exist(t_command *cmd, char **envp);

#endif