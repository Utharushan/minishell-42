/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_management.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:30:00 by ebella            #+#    #+#             */
/*   Updated: 2025/07/06 14:31:46 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_info	*init_info(t_command *cmds, t_env *env)
{
	t_info	*info;

	info = malloc(sizeof(t_info));
	if (!info)
		return (NULL);
	info->cmd_count = count_cmds(cmds);
	info->pid = malloc(sizeof(pid_t) * info->cmd_count);
	if (!info->pid)
	{
		free(info);
		return (NULL);
	}
	info->in_fd = STDIN_FILENO;
	info->cmd_index = 0;
	info->env = env;
	info->cmds = cmds;
	info->first_cmds = cmds;
	return (info);
}

void	free_info(t_info *info)
{
	if (!info)
		return ;
	if (info->pid)
		free(info->pid);
	if (info->first_cmds)
	{
		close_all_heredoc_fds(info->first_cmds);
		free_command_list(info->first_cmds);
	}
	free(info);
}

void	free_info_child(t_info *info)
{
	if (!info)
		return ;
	if (info->pid)
		free(info->pid);
	if (info->first_cmds)
	{
		close_all_heredoc_fds(info->first_cmds);
		free_command_list(info->first_cmds);
	}
	free(info);
}

void	setup_child_fds(t_command *cmds, t_info *info)
{
	if (info->in_fd != 0)
	{
		if (dup2(info->in_fd, STDIN_FILENO) == -1)
		{
			close(info->in_fd);
			return ;
		}
		close(info->in_fd);
	}
	if (cmds->next_op == OP_PIPE)
	{
		if (dup2(info->pipe_fd[1], STDOUT_FILENO) == -1)
		{
			close(info->pipe_fd[0]);
			close(info->pipe_fd[1]);
			return ;
		}
		close(info->pipe_fd[0]);
		close(info->pipe_fd[1]);
	}
}

void	free_info_and_exit(t_info *info, int exit_code)
{
	if (info)
	{
		if (info->env)
			free_env_list(info->env);
		free_info_child(info);
	}
	exit(exit_code);
}
