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
	t_token *prev = NULL;
	t_token *err_tok = NULL;
	while (tokens)
	{
		if ((tokens->type == TOKEN_PIPE || tokens->type == TOKEN_AMP
			|| tokens->type == TOKEN_SEMICOLON) &&
			(!prev || !tokens->next || tokens->next->type == TOKEN_EOF))
		{
			err_tok = tokens;
			break ;
		}
		if (tokens->type == TOKEN_PIPE)
		{
			if (tokens->next && tokens->next->type == TOKEN_PIPE)
			{
				err_tok = tokens->next;
				break ;
			}
		}
		if (tokens->type == TOKEN_AMP)
		{
			if (tokens->next && tokens->next->type == TOKEN_AMP)
			{
				err_tok = tokens->next;
				break ;
			}
		}
		if (tokens->type == TOKEN_SEMICOLON)
		{
			if (tokens->next && tokens->next->type == TOKEN_SEMICOLON)
			{
				err_tok = tokens->next;
				break ;
			}
		}
		prev = tokens;
		tokens = tokens->next;
	}
	if (err_tok)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
		if (err_tok->type == TOKEN_PIPE)
			ft_putstr_fd("|", 2);
		else if (err_tok->type == TOKEN_AMP)
			ft_putstr_fd("&", 2);
		else if (err_tok->type == TOKEN_SEMICOLON)
			ft_putstr_fd(";", 2);
		ft_putstr_fd("'\n", 2);
		return (1);
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
