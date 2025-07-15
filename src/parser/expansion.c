/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 13:31:31 by tuthayak          #+#    #+#             */
/*   Updated: 2025/05/30 13:31:31 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_env_value(t_env *env, const char *name)
{
	while (env)
	{
		if (ft_strcmp(env->name, name) == 0)
			return (env->value);
		env = env->next;
	}
	return ("");
}

static char	*expand_char_invalid(void)
{
	char	*dollar;

	dollar = malloc(2);
	if (dollar)
	{
		dollar[0] = '$';
		dollar[1] = 0;
	}
	return (dollar);
}

static char	*expand_var_pwd(t_env *env, char *name)
{
	char	cwd[4096];
	char	*pwd_str;

	if (ft_strcmp(name, "PWD") == 0 && getcwd(cwd, sizeof(cwd)))
	{
		pwd_str = ft_strjoin("PWD=", cwd);
		add_new_node(env, pwd_str);
		free(pwd_str);
		return (ft_strdup(cwd));
	}
	return (NULL);
}

static char	*expand_var_value(t_env *env, char *name)
{
	char	*val;
	char	*result;

	val = get_env_value(env, name);
	result = expand_var_pwd(env, name);
	if (result)
	{
		free(name);
		return (result);
	}
	free(name);
	if (val)
		return (ft_strdup(val));
	return (ft_strdup(""));
}

char	*expand_var(const char *str, int *i, t_env *env, int last_status)
{
	int		start;
	char	*name;

	start = ++(*i);
	if (str[start] == '?')
	{
		(*i)++;
		return (ft_itoa(last_status));
	}
	if (!ft_isalpha(str[start]) && str[start] != '_')
		return (expand_char_invalid());
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	return (expand_var_value(env, name));
}
