/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 12:45:03 by ebella            #+#    #+#             */
/*   Updated: 2025/07/09 16:33:03 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*new_env_node(char *name, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = ft_strdup(name);
	if (!new->name)
	{
		free(new);
		return (NULL);
	}
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(new->name);
		free(new);
		return (NULL);
	}
	new->next = NULL;
	return (new);
}

void	add_env_back(t_env **env, t_env *new)
{
	t_env	*tmp;

	if (!*env)
	{
		*env = new;
		return ;
	}
	tmp = *env;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*init_env(char **envp, t_env *env)
{
	int		i;
	char	*equal_position;
	char	*name;
	char	*value;
	t_env	*new_env;

	i = 0;
	while (envp[i])
	{
		equal_position = ft_strchr(envp[i], '=');
		if (equal_position)
		{
			name = ft_substr(envp[i], 0, equal_position - envp[i]);
			if (!name)
			{
				free(equal_position);
				return (env);
			}
			value = ft_strdup(equal_position + 1);
			if (!value)
			{
				free(equal_position);
				free(name);
				return (env);
			}
			new_env = new_env_node(name, value);
			free(name);
			free(value);
			if (!new_env)
				continue ;
			add_env_back(&env, new_env);
		}
		i++;
	}
	return (env);
}

int	ft_env(t_env *env)
{
	if (!env)
		return (1);
	while (env)
	{
		if (env->name && env->value)
		{
			ft_putstr_fd(env->name, 1);
			ft_putchar_fd('=', 1);
			ft_putstr_fd(env->value, 1);
			ft_putchar_fd('\n', 1);
		}
		env = env->next;
	}
	return (0);
}
