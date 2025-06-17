/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:18:30 by ebella            #+#    #+#             */
/*   Updated: 2025/06/17 17:03:03 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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

//chiant a separer, je ferais ca en dernier...
int exec_command(t_command *cmd, t_env *env)
{
	int i;
	char *full_path;
	char **envp;
	char **path_dirs;

	envp = env_to_envp(env);
	path_dirs = get_path_dirs(env);
	if (ft_strchr(cmd->args[0], '/'))
	{
		execve(cmd->args[0], cmd->args, envp);
		perror("execve");
		free(envp);
		return (127);
	}
	i = 0;
	while (path_dirs && path_dirs[i])
	{
		full_path = build_full_path(path_dirs[i], cmd->args[0]);
		if (!full_path)
			break;
		if (access(full_path, F_OK | X_OK) == 0)
		{
			execve(full_path, cmd->args, envp);
			perror("execve");
			free(full_path);
			break;
		}
		free(full_path);
		i++;
	}
	if (path_dirs)
	{
		i = 0;
		while (path_dirs[i])
			free(path_dirs[i++]);
		free(path_dirs);
	}
	if (envp)
	{
		i = 0;
		while (envp[i])
			free(envp[i++]);
		free(envp);
	}
	perror(cmd->args[0]);
	return (127);
}
