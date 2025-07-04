/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:46:46 by ebella            #+#    #+#             */
/*   Updated: 2025/07/05 00:45:57 by ebella           ###   ########.fr       */
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
	if (*in_fd != 0 && *in_fd != -1)
		close(*in_fd);
	if (cmds->next_op == OP_PIPE)
	{
		if (pipe_fd[1] != -1)
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
			if ((redirection->type == TOKEN_HEREDOC)
				&& redirection->heredoc_fd >= 0)
			{
				close(redirection->heredoc_fd);
				redirection->heredoc_fd = -1;
			}
			redirection = redirection->next;
		}
		command = command->next;
	}
}

void	close_unused_heredoc_fds(t_command *current_cmd, t_command *all_cmds)
{
	t_command	*cmd;
	t_redir		*redir;

	cmd = all_cmds;
	while (cmd)
	{
		if (cmd != current_cmd)
		{
			redir = cmd->redir;
			while (redir)
			{
				if ((redir->type == TOKEN_HEREDOC) && redir->heredoc_fd >= 0)
				{
					close(redir->heredoc_fd);
					redir->heredoc_fd = -1;
				}
				redir = redir->next;
			}
		}
		cmd = cmd->next;
	}
}
