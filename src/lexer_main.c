/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_main.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:39:49 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/31 13:39:49 by tuthayak         ###   ########.fr       */
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
