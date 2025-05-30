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

/*
Checks the token list for syntax errors related to redirections and pipes.
Returns 1 if a syntax error is found, 0 otherwise.
*/
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

/*
Handles input and output redirections for a command.
Updates the command's input/output fields based on the redirection type.
Advances the token pointer to the next token.
*/
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
