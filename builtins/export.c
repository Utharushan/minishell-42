/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:09:43 by ebella            #+#    #+#             */
/*   Updated: 2025/05/28 15:19:24 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	env_found(t_env *env, char *name)
{
	while (env)
	{
		if (!ft_strcmp(name, env->name))
			return (1);
		env = env->next;
	}
	return (0);
}
void	parse_arg(char *name, char *value, t_env *env)
{
	while (env)
	{
		if (!ft_strcmp(name, env->name))
		{
			env->value = value;
			break ;
		}
		env = env->next;
	}
}

int	ft_export(t_env *env, char *str)
{
	int		i;
	t_env	*new_env;
	char	*value;
	char	*name;
	char	*equal_position;

	i = 0;
	if (!env || !str)
		return (1);
	equal_position = ft_strchr(str, '=');
	name = ft_substr(str, 0, equal_position - str);
	value = ft_strdup(equal_position);
	if (env_found(env, name) && equal_position)
		parse_arg(name, value, env);
	else
	{
		new_env = new_env_node(name, value);
		add_env_back(&env, new_env);
	}
	return (0);
}
