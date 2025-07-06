/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:39:04 by ebella            #+#    #+#             */
/*   Updated: 2025/07/06 02:04:37 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_child_process(t_command *cmds, t_info *info)
{
	setup_child_fds(cmds, info);
	close_unused_heredoc_fds(cmds, info->first_cmds);
	if (command_redirections(cmds) == 0)
	{
		close_all_heredoc_fds(info->first_cmds);
		free_info(info);
		exit(130);
	}
	close_all_heredoc_fds(info->first_cmds);
	if (is_builtins(cmds) == 0)
		exec_child_builtin(cmds, info);
	child_process_signals();
	exec_child_external(cmds, info);
}

void	launch_child_process(t_command *cmds, t_info *info)
{
	create_pipe(cmds, info->pipe_fd);
	info->pid[info->cmd_index] = create_child_process();
	if (info->pid[info->cmd_index++] == 0)
		handle_child_process(cmds, info);
	else
		close_fd(&info->in_fd, cmds, info->pipe_fd);
}

void	exec_pipe(t_command *cmds, t_info *info)
{
	while (cmds)
	{
        if (find_cmd_in_path(cmds, info->env) && is_builtins(cmds))
	    {
	    	ft_putstr_fd(cmds->args[0], 2);
	    	ft_putstr_fd(": command not found\n", 2);
	    	singleton(1, 127);
	    	free_command_list(cmds);
	    	return ;
	    }
		launch_child_process(cmds, info);
		cmds = cmds->next;
	}
	wait_for_pids(info->first_cmds, info->pid);
}

int	handle_parent_builtin_if_needed(t_command *cmds, t_info *info)
{
	if (!cmds->next && is_parent_builtin(cmds))
	{
		run_builtins(cmds, info);
		return (1);
	}
	return (0);
}

void	run_pipe(t_command *cmds, t_env *env)
{
	t_info	*info;

	info = init_info(cmds, env);
	if (!info)
		return ;
	if (handle_parent_builtin_if_needed(cmds, info))
	{
		free_info(info);
		return ;
	}
	exec_pipe(cmds, info);
	free_info(info);
}
