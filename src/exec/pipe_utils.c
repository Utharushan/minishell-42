/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:46:46 by ebella            #+#    #+#             */
/*   Updated: 2025/06/26 12:40:57 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_cmds(t_command *cmds)
{
	int	i;

	if (!cmds)
		return (0);
	i = 0;
	while (cmds)
	{
		i++;
		cmds = cmds->next;
	}
	return (i);
}

void	close_fd(int *in_fd, t_command *cmds, int *pipe_fd)
{
	if (*in_fd != 0)
		close(*in_fd);
	if (cmds->next_op == OP_PIPE)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
}

void	close_all_heredoc_fds(t_command *cmds)
{
	t_command	*command;
	t_redir		*redirection;

	command = cmds;
	if (!command || !command->redir)
		return ;
	while (command)
	{
		redirection = command->redir;
		while (redirection)
		{
			if ((redirection->type == TOKEN_HEREDOC
					|| redirection->type == TOKEN_REDIRECT_IN
					|| redirection->type == TOKEN_REDIRECT_OUT
					|| redirection->type == TOKEN_REDIRECT_APPEND)
				&& redirection->heredoc_fd > 0)
			{
				close(redirection->heredoc_fd);
				redirection->heredoc_fd = -1;
			}
			redirection = redirection->next;
		}
		command = command->next;
	}
}
