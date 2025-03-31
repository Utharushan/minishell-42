#include "minishell.h"

t_token_type	get_token_type(char *input, int *i)
{
	if (input[*i] == '|')
	{
		return (TOKEN_PIPE);
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			(*i)++;
			return (TOKEN_HEREDOC);
		}
		return (TOKEN_REDIRECT_IN);
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			(*i)++;
			return (TOKEN_REDIRECT_APPEND);
		}
		return (TOKEN_REDIRECT_OUT);
	}
	return (TOKEN_WORD);
}

void	extract_word(char *input, int *i, t_token **tokens)
{
	int	start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|' &&
		   input[*i] != '<' && input[*i] != '>')
	{
		(*i)++;
	}
	add_token(tokens, ft_substr(input, start, *i - start), TOKEN_WORD);
	(*i)--;
}
