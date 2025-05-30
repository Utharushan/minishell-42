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

static char	*expand_var(const char *str, int *i, t_env *env, int last_status)
{
	int		start;
	char	*name;
	char	*val;

	start = ++(*i);
	if (str[start - 1] == '?')
	{
		(*i)++;
		return (ft_itoa(last_status));
	}
	if (!ft_isalpha(str[start]) && str[start] != '_')
	{
		char	*dollar = malloc(2);

		dollar[0] = '$';
		dollar[1] = 0;
		return (dollar);
	}
	while (str[*i] && (ft_isalnum(str[*i]) || str[*i] == '_'))
		(*i)++;
	name = ft_substr(str, start, *i - start);
	val = get_env_value(env, name);
	free(name);
	return (ft_strdup(val ? val : ""));
}

char	*expand_token_value(const char *str, t_env *env, int last_status)
{
	char	*result;
	int		i;
	int		start;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
		{
			start = ++i;
			while (str[i] && str[i] != '\'')
				i++;
			char	*seg = ft_substr(str, start, i - start);

			result = ft_strjoin(result, seg);
			free(seg);
			if (str[i] == '\'')
				i++;
		}
		else if (str[i] == '"')
		{
			start = ++i;
			char	*dq = ft_strdup("");

			while (str[i] && str[i] != '"')
			{
				if (str[i] == '$')
				{
					char	*exp = expand_var(str, &i, env, last_status);

					dq = ft_strjoin(dq, exp);
					free(exp);
				}
				else
				{
					char	tmp[2] = {str[i++], 0};

					dq = ft_strjoin(dq, tmp);
				}
			}
			result = ft_strjoin(result, dq);
			free(dq);
			if (str[i] == '"')
				i++;
		}
		else if (str[i] == '$')
		{
			char	*exp = expand_var(str, &i, env, last_status);

			result = ft_strjoin(result, exp);
			free(exp);
		}
		else
		{
			start = i;
			while (str[i] && str[i] != '\'' && str[i] != '"' && str[i] != '$')
				i++;
			char	*seg = ft_substr(str, start, i - start);

			result = ft_strjoin(result, seg);
			free(seg);
		}
	}
	return (result);
}

