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

#include "../../includes/minishell.h"

int	check_syntax_errors(t_token *tokens)
{
	t_token	*err_tok;

	err_tok = get_syntax_error_token(tokens);
	if (err_tok)
	{
		print_syntax_error(err_tok);
		return (1);
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
	{
		if (cmd->input)
			free(cmd->input);
		cmd->input = ft_strdup((*tokens)->value);
	}
	else
	{
		if (cmd->output)
			free(cmd->output);
		cmd->output = ft_strdup((*tokens)->value);
		cmd->append = (type == TOKEN_REDIRECT_APPEND);
	}
}

int	validate_quotes(const char *input, int start, char quote)
{
	while (input[start])
	{
		if (input[start] == quote)
			return (1);
		start++;
	}
	return (0);
}
