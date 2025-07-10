/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 10:00:00 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/10 10:00:00 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_invalid_start_end(t_token *tok, t_token *prev)
{
	if ((tok->type == TOKEN_PIPE || tok->type == TOKEN_AMP
			|| tok->type == TOKEN_SEMICOLON)
		&& (!prev || !tok->next || tok->next->type == TOKEN_EOF))
		return (1);
	return (0);
}

static int	is_double_operator(t_token *tok)
{
	if ((tok->type == TOKEN_PIPE || tok->type == TOKEN_SEMICOLON)
		&& tok->next && (tok->next->type == TOKEN_PIPE
			|| tok->next->type == TOKEN_SEMICOLON))
		return (1);
	if ((tok->type == TOKEN_AMP || tok->type == TOKEN_SEMICOLON)
		&& tok->next && (tok->next->type == TOKEN_AMP
			|| tok->next->type == TOKEN_SEMICOLON))
		return (1);
	return (0);
}

t_token	*get_syntax_error_token(t_token *tokens)
{
	t_token	*prev;

	prev = NULL;
	while (tokens)
	{
		if (is_invalid_start_end(tokens, prev))
			return (tokens);
		if (is_double_operator(tokens))
			return (tokens->next);
		prev = tokens;
		tokens = tokens->next;
	}
	return (NULL);
}

void	print_syntax_error(t_token *err_tok)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (err_tok->type == TOKEN_PIPE)
		ft_putstr_fd("|", 2);
	else if (err_tok->type == TOKEN_AMP)
		ft_putstr_fd("&", 2);
	else if (err_tok->type == TOKEN_SEMICOLON)
		ft_putstr_fd(";", 2);
	ft_putstr_fd("'\n", 2);
}
