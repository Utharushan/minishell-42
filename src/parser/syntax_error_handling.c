/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_error_handling.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 14:14:44 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/31 14:14:44 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_syntax_errors(t_token *tokens)
{
	while (tokens)
	{
		if ((tokens->type == TOKEN_REDIRECT_IN
				|| tokens->type == TOKEN_REDIRECT_OUT
				|| tokens->type == TOKEN_REDIRECT_APPEND
				|| tokens->type == TOKEN_PIPE)
			&& (!tokens->next || tokens->next->type != TOKEN_WORD))
			return (1);
		tokens = tokens->next;
	}
	return (0);
}

void	handle_redirection(t_command *cmd, t_token **tokens)
{
	t_token_type	type;

	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		return ;
	if (type == TOKEN_REDIRECT_IN)
		cmd->input = (*tokens)->value;
	else
	{
		cmd->output = (*tokens)->value;
		cmd->append = (type == TOKEN_REDIRECT_APPEND);
	}
}
