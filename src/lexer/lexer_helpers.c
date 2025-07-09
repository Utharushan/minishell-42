/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_helpers.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 22:30:00 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/07 23:02:13 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	extract_word_quoted(char *input, int *i, t_token **tokens,
							int has_leading_space)
{
	int			start;
	char		quote;
	t_word_type	word_type;

	quote = input[*i];
	start = *i;
	(*i)++;
	while (input[*i] && input[*i] != quote)
		(*i)++;
	if (input[*i] == quote)
	{
		(*i)++;
		word_type = get_word_type(quote);
		add_token(tokens, (t_token_args){ft_substr(input, start, *i - start),
			TOKEN_WORD, word_type, has_leading_space});
		if (*tokens == NULL)
			return ;
	}
	else
	{
		ft_putstr_fd("minishell: unexpected EOF", 2);
		ft_putchar_fd(quote, 2);
		ft_putstr_fd("'\n", 2);
		*tokens = NULL;
	}
}

void	extract_word_unquoted(char *input, int *i, t_token **tokens,
								int has_leading_space)
{
	int		start;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i])
		&& input[*i] != '\'' && input[*i] != '"'
		&& input[*i] != '|' && input[*i] != '<'
		&& input[*i] != '>' && input[*i] != '&'
		&& input[*i] != '(' && input[*i] != ')'
		&& input[*i] != ';')
		(*i)++;
	add_token(tokens, (t_token_args){ft_substr(input, start, *i - start),
		TOKEN_WORD, WORD_UNQUOTED, has_leading_space});
	if (*tokens == NULL)
		return ;
}

void	handle_token_word(char *input, int *i, t_token **tokens)
{
	extract_word(input, i, tokens);
	if (*tokens == NULL)
		return ;
	(void)input;
}

void	handle_token_operator(char *input, int *i, t_token **tokens,
				t_token_type type)
{
	int				length;
	t_token_args	args;

	length = 1;
	if (type == TOKEN_HEREDOC || type == TOKEN_REDIRECT_APPEND)
	{
		length = 2;
		args.value = ft_substr(input, *i - 1, length);
		args.type = type;
		args.word_type = WORD_UNQUOTED;
		args.has_leading_space = 0;
		add_token(tokens, args);
		*i += 1;
	}
	else
	{
		args.value = ft_substr(input, *i, length);
		args.type = type;
		args.word_type = WORD_UNQUOTED;
		args.has_leading_space = 0;
		add_token(tokens, args);
		*i += length;
	}
}

t_word_type	get_word_type(char quote)
{
	if (quote == '\'')
		return (WORD_SINGLE_QUOTED);
	return (WORD_DOUBLE_QUOTED);
}
