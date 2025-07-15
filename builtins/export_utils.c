/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 07:33:20 by ebella            #+#    #+#             */
/*   Updated: 2025/07/13 15:57:20 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	export_error(char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

char	*find_plus_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i + 1])
	{
		if (str[i] == '+' && str[i + 1] == '=')
			return (str + i);
		i++;
	}
	return (NULL);
}

void	append_env_value(t_env *env, char *name, char *to_append)
{
	char	*tmp;

	while (env)
	{
		if (!ft_strcmp(name, env->name))
		{
			tmp = env->value;
			env->value = ft_strjoin(env->value, to_append);
			free(tmp);
			break ;
		}
		env = env->next;
	}
}

static int	parse_plus_equal_syntax(char *arg, char **name, char **value)
{
	char	*plus_equal;

	plus_equal = find_plus_equal(arg);
	if (!plus_equal)
		return (1);
	*name = ft_substr(arg, 0, plus_equal - arg);
	if (!*name)
		return (1);
	*value = ft_strdup(plus_equal + 2);
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
	return (0);
}

int	handle_plus_equal(t_env *env, char *arg)
{
	char	*name;
	char	*value;
	t_env	*new_env;

	if (parse_plus_equal_syntax(arg, &name, &value) != 0)
		return (1);
	if (env_found(env, name))
		append_env_value(env, name, value);
	else
	{
		new_env = new_env_node(name, value);
		if (!new_env)
		{
			free(name);
			free(value);
			return (1);
		}
		add_env_back(&env, new_env);
	}
	free(name);
	free(value);
	return (0);
}
