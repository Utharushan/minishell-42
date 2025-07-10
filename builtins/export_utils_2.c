/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils_2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 23:07:17 by ebella            #+#    #+#             */
/*   Updated: 2025/07/10 23:26:46 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	add_new_node(t_env *env, char *args)
{
	char	*name;
	char	*value;
	int		parse_result;

	parse_result = parse_export(args, &name, &value);
	if (parse_result != 2)
		return (parse_result);
	return (update_or_create_env(env, name, value));
}

int	update_or_create_env(t_env *env, char *name, char *value)
{
	t_env	*cur;
	t_env	*new_env;

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

int	export_arg(t_env *env, char *arg)
{
	if (find_plus_equal(arg))
	{
		if (handle_plus_equal(env, arg))
			return (1);
	}
	else if (ft_strchr(arg, '='))
	{
		if (add_new_node(env, arg))
			return (1);
	}
	else
	{
		if (argv_parsed(arg) != 0)
		{
			export_error(arg);
			return (1);
		}
	}
	return (0);
}
