/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:27:01 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/15 21:59:10 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	parse_first_word(t_command *cmd, t_token **tokens,
	t_env *env, t_command *head)
{
	if (!add_argument_concat(cmd, tokens, env))
		return (free_command_list(head), 0);
	if (cmd->args && cmd->args[0] && is_directory(cmd->args[0]))
	{
		ft_putstr_fd(cmd->args[0], 2);
		ft_putstr_fd(": Is a directory\n", 2);
		return (free_command_list(head), 0);
	}
	return (1);
}

static int	parse_pipe_token(t_command **cmd, t_token **tokens,
	t_env *env, t_command *head)
{
	*cmd = handle_pipe(*cmd);
	*tokens = (*tokens)->next;
	if (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		if (!add_argument(*cmd, (*tokens)->value, (*tokens)->word_type, env))
			return (free_command_list(head), 0);
		*tokens = (*tokens)->next;
	}
	return (1);
}

static int	parse_redirection_token(t_command *cmd, t_token **tokens,
	t_command *head)
{
	if ((*tokens)->type == TOKEN_REDIRECT_IN
		|| (*tokens)->type == TOKEN_REDIRECT_OUT
		|| (*tokens)->type == TOKEN_REDIRECT_APPEND)
	{
		if (!handle_redirection_tokens(cmd, tokens, head))
			return (0);
	}
	else if ((*tokens)->type == TOKEN_HEREDOC)
	{
		if (!handle_heredoc_tokens(cmd, tokens, head))
			return (0);
	}
	return (1);
}

static int	parse_token_loop(t_command *cmd, t_token **tokens, t_env *env,
	t_command *head)
{
	while (*tokens && (*tokens)->type != TOKEN_EOF)
	{
		if ((*tokens)->type == TOKEN_WORD)
		{
			if (!add_argument_concat(cmd, tokens, env))
				return (free_command_list(head), 0);
			continue ;
		}
		else if ((*tokens)->type == TOKEN_PIPE
			|| (*tokens)->type == TOKEN_SEMICOLON)
		{
			if (!parse_pipe_token(&cmd, tokens, env, head))
				return (0);
			continue ;
		}
		else if (!parse_redirection_token(cmd, tokens, head))
			return (0);
		*tokens = (*tokens)->next;
	}
	return (1);
}

t_command	*parse_tokens(t_token *tokens, t_env *env)
{
	t_command	*cmd;
	t_command	*head;
	t_token		*tok_ptr;

	if (check_syntax_errors(tokens))
		return (NULL);
	cmd = new_command();
	head = cmd;
	tok_ptr = tokens;
	if (tok_ptr && tok_ptr->type == TOKEN_WORD)
	{
		if (!parse_first_word(cmd, &tok_ptr, env, head))
			return (NULL);
	}
	if (!parse_token_loop(cmd, &tok_ptr, env, head))
		return (NULL);
	if (cmd->args && cmd->args[0] && !ft_strncmp(cmd->args[0], "cd", 3)
		&& cmd->args[2])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		free_command_list(head);
		return (NULL);
	}
	return (head);
}
