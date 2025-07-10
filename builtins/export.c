/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:09:43 by ebella            #+#    #+#             */
/*   Updated: 2025/07/10 23:23:01 by ebella           ###   ########.fr       */
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

int	argv_parsed(char *name)
{
	int	i;

	if (!name || !name[0])
		return (1);
	if (!ft_isalpha(name[0]) && name[0] != '_')
		return (1);
	i = 1;
	while (name[i])
	{
		if (!ft_isalpha(name[i]) && !ft_isdigit(name[i]) && name[i] != '_')
			return (1);
		i++;
	}
	return (0);
}

int	parse_export(char *args, char **name, char **value)
{
	char	*equal_position;

	equal_position = ft_strchr(args, '=');
	if (!equal_position)
		return (0);
	*name = ft_substr(args, 0, equal_position - args);
	if (!*name)
		return (1);
	*value = ft_strdup(equal_position + 1);
	if (!*value)
	{
		free(*name);
		return (1);
	}
	if (argv_parsed(*name) != 0)
	{
		free(*name);
		free(*value);
		return (1);
	}
	return (2);
}

int	ft_export(t_env *env, char **args)
{
	int	i;
	int	result;

	i = 0;
	result = 0;
	if (!env)
		return (1);
	while (args[++i])
	{
		if (export_arg(env, args[i]))
			result = 1;
	}
	return (result);
}
