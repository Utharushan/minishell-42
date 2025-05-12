/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:18:30 by ebella            #+#    #+#             */
/*   Updated: 2025/05/09 15:34:07 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int init_command_path(t_command *cmd, char **envp)
{
	int i;

	i = 0;
	if (!cmd || !envp)
		return (1);
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			cmd->path = ft_split(envp[i] + 5, ':');
			if (!cmd->path)
				return (1);
			return (0);
		}
		i++;
	}
	cmd->path = NULL;
	return (0);
}

char *build_full_path(char *dir, char *cmd)
{
	char *tmp;
	char *full_path;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full_path = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full_path);
}

int exec_command(t_command *cmd, char **envp)
{
	int i;
	char *full_path;

	i = 0;
	while (cmd->path && cmd->path[i])
	{
		full_path = build_full_path(cmd->path[i], cmd->args[0]);
		if (!full_path)
			return (1);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			execve(full_path, cmd->args, envp);
			perror("execve");
			free(full_path);
			return (127);
		}
		free(full_path);
		i++;
	}
	perror(cmd->args[0]);
	return (127);
}
