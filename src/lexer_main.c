#include "minishell.h"

static void	handle_token(char *input, int *i, t_token **tokens)
{
	t_token_type type;

	type = get_token_type(input, i);
	if (type == TOKEN_WORD)
	{
		extract_word(input, i, tokens);
	}
	else
	{
		add_token(tokens, ft_substr(input, *i,
			(type == TOKEN_HEREDOC || type == TOKEN_REDIRECT_APPEND) ? 2 : 1), type);
	}
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (input[i])
	{
		if (ft_isspace(input[i]))
		{
			i++;
			continue;
		}
		handle_token(input, &i, &tokens);
		i++;
	}
	add_token(&tokens, NULL, TOKEN_EOF);
	return (tokens);
}