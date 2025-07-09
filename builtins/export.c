/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:09:43 by ebella            #+#    #+#             */
/*   Updated: 2025/07/09 16:29:20 by ebella           ###   ########.fr       */
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

int	add_new_node(t_env *env, char *args)
{
	t_env	*new_env;
	char	*equal_position;
	char	*name;
	char	*value;
	t_env	*cur;

	equal_position = ft_strchr(args, '=');
	if (!equal_position)
		return (0);
	name = ft_substr(args, 0, equal_position - args);
	if (!name)
		return (1);
	value = ft_strdup(equal_position + 1);
	if (!value)
	{
		free(name);
		return (1);
	}
	if (argv_parsed(name) != 0)
	{
		free(name);
		free(value);
		return (1);
	}
	cur = env;
	while (cur)
	{
		if (!ft_strcmp(cur->name, name))
		{
			free(cur->value);
			cur->value = value;
			free(name);
			return (0);
		}
		cur = cur->next;
	}
	new_env = new_env_node(name, value);
	free(name);
	free(value);
	if (!new_env)
		return (1);
	add_env_back(&env, new_env);
	return (0);
}

int	ft_export(t_env *env, char **args)
{
	int	i;

	i = 0;
	if (!env)
		return (1);
	while (args[++i])
	{
		if (find_plus_equal(args[i]))
		{
			if (handle_plus_equal(env, args[i]))
			{
				export_error(args[i]);
				return (1);
			}
		}
		else if (ft_strchr(args[i], '='))
		{
			if (add_new_node(env, args[i]))
			{
				export_error(args[i]);
				return (1);
			}
		}
	}
	return (0);
}
