/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:09:21 by ebella            #+#    #+#             */
/*   Updated: 2025/06/21 12:35:20 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd(t_command *cmds, t_env *env)
{
	char	*path;
	char	*oldpwd;
	char	cwd[PATH_MAX];

	if (cmds->args[1])
		path = cmds->args[1];
	else
		path = ft_getenv(env, "HOME");
	if (!path || chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	oldpwd = ft_getenv(env, "PWD");
	if (!oldpwd)
		return (1);
	ft_setenv(env, "OLDPWD", oldpwd);
	if (getcwd(cwd, sizeof(cwd)))
		ft_setenv(env, "PWD", cwd);
	return (0);
}
