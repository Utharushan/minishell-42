/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 22:30:00 by ebella            #+#    #+#             */
/*   Updated: 2025/07/06 14:30:12 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_child_process(t_command *cmds, t_info *info)
{
	setup_child_fds(cmds, info);
	close_unused_heredoc_fds(cmds, info->first_cmds);
	if (command_redirections(cmds) == 0)
		free_info_and_exit(info, 130);
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

void	exec_child_external(t_command *cmds, t_info *info)
{
	int	exit_code;

	if (!cmds || !cmds->args || !cmds->args[0])
		free_info_and_exit(info, singleton(0, 0));
	if (!ft_strncmp(cmds->args[0], "exit", 5))
		free_info_and_exit(info, singleton(0, 0));
	exit_code = exec_command(cmds, info->env);
	singleton(1, exit_code);
	free_info_and_exit(info, exit_code);
}
