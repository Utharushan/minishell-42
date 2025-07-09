/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/05/30 14:33:45 by tuthayak          #+#    #+#             */
/*   Updated: 2025/05/30 14:33:45 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*ft_getenv(t_env *env, const char *name)
{
	while (env)
	{
		if (!ft_strcmp(env->name, name))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	ft_setenv(t_env *env, const char *name, const char *value)
{
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			free(env->value);
			env->value = ft_strdup(value);
			if (!env->value)
				return ;
			return ;
		}
		env = env->next;
	}
}

int	count_env(t_env *tmp)
{
	int	count;

	count = 0;
	while (tmp)
	{
		count++;
		tmp = tmp->next;
	}
	return (count);
}

void	free_envp(char **envp, int i)
{
	while (i > 0)
		free(envp[--i]);
	free(envp);
}

char	**env_to_envp(t_env *env)
{
	int		count;
	int		i;
	t_env	*tmp;
	char	**envp;
	char	*tmp_str;

	i = 0;
	tmp = env;
	count = count_env(tmp);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		tmp_str = ft_strjoin(tmp->name, "=");
		if (!tmp_str)
			return (free_envp(envp, i), NULL);
		envp[i] = ft_strjoin(tmp_str, tmp->value);
		free(tmp_str);
		if (!envp[i])
			return (free_envp(envp, i), NULL);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}

char	**get_path_dirs(t_env *env)
{
	char	*path_value;
	char	**path_dirs;

	path_value = ft_getenv(env, "PATH");
	if (!path_value)
		return (NULL);
	path_dirs = ft_split(path_value, ':');
	if (!path_dirs)
	{
		free(path_value);
		return (NULL);
	}
	return (path_dirs);
}
