/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:39:52 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/09 21:54:40 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_token(char *input, int *i, t_token **tokens)
{
	t_token_type	type;
	t_token_ctx		ctx;

	set_leading_space(&ctx.has_leading_space, input, *i);
	reset_token_counts(&ctx.old_tokens, &ctx.token_count_before,
		&ctx.token_count_after, *tokens);
	type = get_token_type(input, i);
	if (type == TOKEN_WORD)
		handle_token_word(input, i, tokens);
	else
		handle_token_operator(input, i, tokens, type);
	ctx.tmp = *tokens;
	while (ctx.tmp)
	{
		ctx.token_count_after++;
		ctx.tmp = ctx.tmp->next;
	}
	handle_token_cleanup(tokens, ctx.old_tokens, ctx.token_count_before,
		ctx.token_count_after);
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
		if (tokens == NULL)
			return (NULL);
	}
	add_token(&tokens, (t_token_args){NULL, TOKEN_EOF, WORD_UNQUOTED, 1});
	if (tokens == NULL)
		return (NULL);
	return (tokens);
}

t_token_type	get_token_type(char *input, int *i)
{
	if (input[*i] == '|')
		return (TOKEN_PIPE);
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
	else if (input[*i] == '&')
		return (TOKEN_AMP);
	else if (input[*i] == ';')
		return (TOKEN_SEMICOLON);
	return (TOKEN_WORD);
}

void	extract_word(char *input, int *i, t_token **tokens)
{
	int		has_leading_space;

	has_leading_space = 0;
	if (*i == 0)
		has_leading_space = 0;
	else if (ft_isspace(input[*i - 1]))
		has_leading_space = 1;
	if (input[*i] == '\'' || input[*i] == '"')
		extract_word_quoted(input, i, tokens, has_leading_space);
	else
		extract_word_unquoted(input, i, tokens, has_leading_space);
}

void	set_leading_space(int *has_leading_space, char *input, int i)
{
	*has_leading_space = 0;
	if (i > 0 && ft_isspace(input[i - 1]))
		*has_leading_space = 1;
}
