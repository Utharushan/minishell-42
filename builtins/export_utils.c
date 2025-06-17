/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 07:33:20 by ebella            #+#    #+#             */
/*   Updated: 2025/06/17 15:48:34 by ebella           ###   ########.fr       */
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

int	handle_plus_equal(t_env *env, char *arg)
{
	char	*plus_equal;
	char	*name;
	char	*value;
	t_env	*new_env;

	plus_equal = find_plus_equal(arg);
	if (!plus_equal)
		return (0);
	name = ft_substr(arg, 0, plus_equal - arg);
	value = ft_strdup(plus_equal + 2);
	if (argv_parsed(name) != 0)
	{
		free(name);
		free(value);
		return (1);
	}
	if (env_found(env, name))
		append_env_value(env, name, value);
	else
	{
		new_env = new_env_node(name, value);
		add_env_back(&env, new_env);
	}
	return (0);
}
