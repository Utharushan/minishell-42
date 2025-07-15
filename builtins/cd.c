/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:09:21 by ebella            #+#    #+#             */
/*   Updated: 2025/07/13 16:09:04 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	pwd_init(t_env *env)
{
	char	cwd[PATH_MAX];
	char	*pwd;

	pwd = ft_strjoin("PWD=", getcwd(cwd, sizeof(cwd)));
	if (!pwd)
		return ;
	add_new_node(env, pwd);
	free(pwd);
	ft_setenv(env, "OLDPWD", ft_getenv(env, "PWD"));
}

int	cd_and_update(char *path, t_env *env)
{
	char	cwd[PATH_MAX];

	if (!path || chdir(path) != 0)
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		singleton(1, 1);
		return (1);
	}
	if (getcwd(cwd, sizeof(cwd)))
		ft_setenv(env, "PWD", cwd);
	else
	{
		ft_putstr_fd("chdir: error retrieving current directory\n", 2);
		return (1);
	}
	return (0);
}

int	ft_cd(t_command *cmds, t_env *env)
{
	char	*path;
	char	*oldpwd;

	if (cmds->args[1])
		path = cmds->args[1];
	else
		path = ft_getenv(env, "HOME");
	oldpwd = ft_getenv(env, "PWD");
	if (!oldpwd)
		pwd_init(env);
	else
		ft_setenv(env, "OLDPWD", oldpwd);
	return (cd_and_update(path, env));
}
