/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 20:48:14 by ebella            #+#    #+#             */
/*   Updated: 2025/07/13 12:56:20 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	check_absolute_path(char *cmd_path)
{
	if (access(cmd_path, F_OK | X_OK) == 0)
		return (0);
	return (1);
}

static void	free_path_dirs(char **path_dirs)
{
	int	i;

	if (!path_dirs)
		return ;
	i = 0;
	while (path_dirs[i])
		free(path_dirs[i++]);
	free(path_dirs);
}

static int	search_in_path_dirs(char **path_dirs, char *cmd_name)
{
	char	*full_path;
	int		i;

	i = 0;
	while (path_dirs[i])
	{
		full_path = build_full_path(path_dirs[i], cmd_name);
		if (!full_path)
			return (1);
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free(full_path);
			return (0);
		}
		free(full_path);
		i++;
	}
	return (1);
}

int	find_cmd_in_path(t_command *cmd, t_env *env)
{
	char	**path_dirs;
	int		result;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strchr(cmd->args[0], '/'))
		return (check_absolute_path(cmd->args[0]));
	path_dirs = get_path_dirs(env);
	if (!path_dirs)
		return (1);
	result = search_in_path_dirs(path_dirs, cmd->args[0]);
	free_path_dirs(path_dirs);
	return (result);
}
