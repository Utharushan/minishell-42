/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:09:21 by ebella            #+#    #+#             */
/*   Updated: 2025/06/25 19:13:30 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_cd(t_command *cmds, t_env *env)
{
	char	*path;
	char	*oldpwd;
	char	cwd[PATH_MAX];
	char	*pwd;

	if (cmds->args[1])
		path = cmds->args[1];
	else
		path = ft_getenv(env, "HOME");
	oldpwd = ft_getenv(env, "PWD");
	if (!oldpwd)
	{
		pwd = ft_strjoin("PWD=", getcwd(cwd, sizeof(cwd)));
		add_new_node(env, pwd);
		free(pwd);
		ft_setenv(env, "OLDPWD", ft_getenv(env, "PWD"));
	}else
		ft_setenv(env, "OLDPWD", oldpwd);
	if (!path || chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
		ft_setenv(env, "PWD", cwd);
	return (0);
}
