/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:09:21 by ebella            #+#    #+#             */
/*   Updated: 2025/05/30 14:31:04 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd(t_command *cmds, t_env *env)
{
	char	*path;
	char	*oldpwd;
	char	cwd[PATH_MAX];

	oldpwd = getcwd(NULL, 0);
	if (cmds->args[1])
		path = cmds->args[1];
	else
		path = ft_getenv(env, "HOME");

	if (!path || chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		free(oldpwd);
		return (1);
	}
	ft_setenv(env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		ft_setenv(env, "PWD", cwd);
	free(oldpwd);
	return (0);
}

