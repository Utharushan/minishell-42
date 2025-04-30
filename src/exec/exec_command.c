/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:18:30 by ebella            #+#    #+#             */
/*   Updated: 2025/04/30 13:52:48 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	command_exist(t_command *cmd, char **envp)
{
	int		i;
	char	*tmp;
	char	**tmp2;

	i = 0;
	tmp2 = ft_split(cmd->args[0], ' ');
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			cmd->path = ft_split(envp[i] + 5, ':');
			break ;
		}
		i++;
	}
	i = 0;
	while (cmd->path[i])
	{
		tmp = ft_strjoin(cmd->path[i], "/");
		tmp = ft_strjoin(tmp, tmp2[0]);
		free(tmp);
		i++;
	}
	return (0);
}

int	exec_command(t_command *cmd, char **envp)
{
	int		i;
	char	*tmp;
	char	*full_path;

	i = 0;
	while (cmd->path[i])
	{
		tmp = ft_strjoin(cmd->path[i], "/");
		full_path = ft_strjoin(tmp, cmd->args[0]);
		free(tmp);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			execve(full_path, cmd->args, envp);
			perror("Error:");
			free(full_path);
			return (1);
		}
		free(full_path);
		i++;
	}
	perror("Error:");
	return (0);
}
