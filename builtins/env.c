/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 12:45:03 by ebella            #+#    #+#             */
/*   Updated: 2025/06/20 13:23:19 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_env	*new_env_node(char *name, char *value)
{
	t_env	*new;

	new = malloc(sizeof(t_env));
	if (!new)
		return (NULL);
	new->name = name;
	new->value = value;
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
			value = ft_strdup(equal_position + 1);
			new_env = new_env_node(name, value);
			if (!new_env)
			{
				free(name);
				free(value);
			}
			add_env_back(&env, new_env);
		}
		i++;
	}
	return (env);
}

void	ft_env(t_env *env)
{
	if (!env)
		return ;
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
}
