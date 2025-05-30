/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:33:45 by tuthayak          #+#    #+#             */
/*   Updated: 2025/05/30 14:33:45 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char *ft_getenv(t_env *env, const char *name)
{
	while (env)
	{
		if (!ft_strcmp(env->name, name))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void ft_setenv(t_env *env, const char *name, const char *value)
{
	while (env)
	{
		if (!ft_strcmp(env->name, name))
		{
			free(env->value);
			env->value = ft_strdup(value);
			return;
		}
		env = env->next;
	}
}