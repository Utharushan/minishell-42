/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:27:01 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/11 22:16:26 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*parse_tokens(t_token *tokens, t_env *env)
{
	t_command	*cmd;
	t_command	*head;

	if (check_syntax_errors(tokens))
		return (NULL);
	cmd = new_command();
	head = cmd;
	if (tokens && tokens->type == TOKEN_WORD)
	{
		if (!add_argument_concat(cmd, &tokens, env))
			return (free_command_list(head), NULL);
		if (cmd->args && cmd->args[0] && is_directory(cmd->args[0]))
		{
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd(": Is a directory\n", 2);
			return (free_command_list(head), NULL);
		}
	}
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_WORD)
		{
			if (!add_argument_concat(cmd, &tokens, env))
				return (free_command_list(head), NULL);
			continue ;
		}
		else if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_SEMICOLON)
		{
			cmd = handle_pipe(cmd);
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				if (!add_argument(cmd, tokens->value, tokens->word_type, env))
					return (free_command_list(head), NULL);
				tokens = tokens->next;
			}
			continue ;
		}
		else if (tokens->type == TOKEN_REDIRECT_IN
			|| tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_REDIRECT_APPEND)
		{
			if (!handle_redirection_tokens(cmd, &tokens, head))
				return (NULL);
		}
		else if (tokens->type == TOKEN_HEREDOC)
		{
			if (!handle_heredoc_tokens(cmd, &tokens, head))
				return (NULL);
		}
		tokens = tokens->next;
	}
	return (head);
}
