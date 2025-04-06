/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:39:52 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/31 13:39:52 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	handle_token(char *input, int *i, t_token **tokens)
{
	t_token_type	type;
	int				length;

	type = get_token_type(input, i);
	if (type == TOKEN_WORD)
	{
		extract_word(input, i, tokens);
	}
	else
	{
		if (type == TOKEN_HEREDOC || type == TOKEN_REDIRECT_APPEND)
			length = 2;
		else
			length = 1;
		add_token(tokens, ft_substr(input, *i, length), type);
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
			continue ;
		}
		handle_token(input, &i, &tokens);
		i++;
	}
	add_token(&tokens, NULL, TOKEN_EOF);
	return (tokens);
}

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
	while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
		&& input[*i] != '<' && input[*i] != '>')
	{
		(*i)++;
	}
	add_token(tokens, ft_substr(input, start, *i - start), TOKEN_WORD);
	(*i)--;
}
