/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:09:43 by ebella            #+#    #+#             */
/*   Updated: 2025/05/30 11:21:43 by ebella           ###   ########.fr       */
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

int	argv_parsed(char *name, char *args)
{
	int	i;

	i = 0;
	while (args[i] && name[i])
	{
		if (!ft_isalpha(name[0]))
		{
			if ((name[0] != '_' && !ft_isalpha(name[0])))
				return (1);
		}
		else if ((!ft_isalpha(name[i]) && !ft_isdigit(name[i])))
			return (1);
		i++;
	}
	return (0);
}
int	ft_export(t_env *env, char **args)
{
	int		i;
	t_env	*new_env;
	char	*value;
	char	*name;
	char	*equal_position;

	i = 1;
	if (!env)
		return (1);
	while (args[i])
	{
		equal_position = ft_strchr(args[i], '=');
		if (equal_position)
		{
			name = ft_substr(args[i], 0, equal_position - args[i]);
			value = ft_strdup(equal_position + 1);
			if (env_found(env, name) && equal_position && argv_parsed(name,
					args[i]) == 0)
				parse_arg(name, value, env);
			else if (argv_parsed(name, args[i]) == 0)
			{
				new_env = new_env_node(name, value);
				add_env_back(&env, new_env);
			}
		}
		i++;
	}
	return (0);
}
