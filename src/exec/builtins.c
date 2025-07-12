/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:44:43 by ebella            #+#    #+#             */
/*   Updated: 2025/07/10 22:11:28 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	run_builtins(t_command *cmds, t_info *info)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return ;
	if (!ft_strncmp(cmds->args[0], "cd", 3))
		g_signal_status = ft_cd(cmds, info->env);
	else if (!ft_strncmp(cmds->args[0], "echo", 5))
	{
		if (cmds->args[1] && !ft_strncmp(cmds->args[1], "$?", 3))
		{
			ft_putnbr_fd(g_signal_status, 1);
			ft_putchar_fd('\n', 1);
		}
		else
			g_signal_status = ft_echo(cmds);
	}
	else if (!ft_strncmp(cmds->args[0], "pwd", 4))
		g_signal_status = ft_pwd();
	else if (!ft_strncmp(cmds->args[0], "env", 4))
		g_signal_status = ft_env(info->env);
	else if (!ft_strncmp(cmds->args[0], "export", 7))
		g_signal_status = ft_export(info->env, cmds->args);
	else if (!ft_strncmp(cmds->args[0], "unset", 6))
		g_signal_status = ft_unset(&info->env, cmds->args[1]);
	else if (!ft_strncmp(cmds->args[0], "exit", 5))
		ft_exit(cmds->args, info);
}

int	is_builtins(t_command *cmds)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (1);
	if (!ft_strncmp(cmds->args[0], "cd", 3))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "echo", 5))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "pwd", 4))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "env", 4))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "export", 7))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "unset", 6))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "exit", 5))
		return (0);
	else
		return (1);
}

int	is_parent_builtin(t_command *cmds)
{
	if (!cmds->args || !cmds->args[0])
		return (0);
	if (!ft_strncmp(cmds->args[0], "cd", 3))
		return (1);
	else if (!ft_strncmp(cmds->args[0], "export", 7))
		return (1);
	else if (!ft_strncmp(cmds->args[0], "unset", 6))
		return (1);
	else if (!ft_strncmp(cmds->args[0], "exit", 5))
		return (1);
	return (0);
}

void	exec_child_builtin(t_command *cmds, t_info *info)
{
	if (is_parent_builtin(cmds))
		free_info_and_exit(info, 0);
	else
	{
		run_builtins(cmds, info);
		free_info_and_exit(info, g_signal_status);
	}
}
