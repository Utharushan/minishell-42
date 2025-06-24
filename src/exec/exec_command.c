/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/29 11:18:30 by ebella            #+#    #+#             */
/*   Updated: 2025/06/24 09:13:02 by ebella           ###   ########.fr       */
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

int try_exec_direct(char **args, char **envp)
{
	if (ft_strchr(args[0], '/'))
	{
		execve(args[0], args, envp);
		perror("execve");
		return (127);
	}
	return (-1);
}

int try_exec_path(char **args, char **envp, char **path_dirs)
{
	int i;
	char *full_path;

	i = 0;
	while (path_dirs && path_dirs[i])
	{
		full_path = build_full_path(path_dirs[i], args[0]);
		if (!full_path)
			break;
		if (access(full_path, F_OK | X_OK) == 0)
		{
			execve(full_path, args, envp);
			perror("execve");
			free(full_path);
			return (127);
		}
		free(full_path);
		i++;
	}
	return (-1);
}

void free_str_array(char **envp)
{
	int i;

	i = 0;
	if (!envp)
		return;
	while (envp[i])
		free(envp[i++]);
	free(envp);
}

int exec_command(t_command *cmd, t_env *env)
{
	char **envp;
	char **path_dirs;
	int ret;

	envp = env_to_envp(env);
	path_dirs = get_path_dirs(env);
	ret = try_exec_direct(cmd->args, envp);
	if (ret != -1)
	{
		free_str_array(envp);
		return (ret);
	}
	ret = try_exec_path(cmd->args, envp, path_dirs);
	free_str_array(path_dirs);
	free_str_array(envp);
	if (ret != -1)
		return (ret);
	perror(cmd->args[0]);
	return (127);
}
