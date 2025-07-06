/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_execution.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:30:00 by ebella            #+#    #+#             */
/*   Updated: 2025/07/06 14:20:08 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	exec_pipe(t_command *cmds, t_info *info)
{
	while (cmds)
	{
		launch_child_process(cmds, info);
		cmds = cmds->next;
	}
	wait_for_pids(info->first_cmds, info->pid);
}

int	handle_parent_builtin_if_needed(t_command *cmds, t_info *info)
{
	if (!cmds->next && is_builtins(cmds) == 0 && !cmds->redir)
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
