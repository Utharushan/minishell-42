# ifndef MINISHELL_H
# define MINISHELL_H


#include <stddef.h>

typedef struct	s_env
{
	char			*name;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef enum	e_token_type {
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,
	TOKEN_EOF
}	t_token_type;

typedef struct	s_token {
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

# endif