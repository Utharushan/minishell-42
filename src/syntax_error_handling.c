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

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = (NULL);
	cmd->next = (NULL);
	cmd->input = (NULL);
	cmd->output = (NULL);
	cmd->append = 0;
	return (cmd);
}

void	add_argument(t_command *cmd, char *arg)
{
	int		count;
	char	**new_args;
	int		i;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return ;
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = arg;
	new_args[count + 1] = (NULL);
	free(cmd->args);
	cmd->args = new_args;
}

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
	*tokens = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
		return ;
	if ((*tokens)->type == TOKEN_REDIRECT_IN)
		cmd->input = (*tokens)->value;
	else
	{
		cmd->output = (*tokens)->value;
		cmd->append = ((*tokens)->type == TOKEN_REDIRECT_APPEND);
	}
}

t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd;
	t_command	*head;

	if (check_syntax_errors(tokens))
		return ((NULL));
	cmd = new_command();
	head = cmd;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_WORD)
			add_argument(cmd, tokens->value);
		else if (tokens->type == TOKEN_PIPE)
		{
			cmd->next = new_command();
			cmd = cmd->next;
		}
		else if (tokens->type == TOKEN_REDIRECT_IN
			|| tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_REDIRECT_APPEND)
			handle_redirection(cmd, &tokens);
		tokens = tokens->next;
	}
	return (head);
}
