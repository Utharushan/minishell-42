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

static char	*expand_var(const char *name, t_env *env, int last_status)
{
	char	*val;

	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(last_status));
	
	val = get_env_value(env, name);
	return (ft_strdup(val ? val : ""));
}

char	*expand_token_value(const char *str, t_env *env, int last_status, t_word_type word_type)
{
	(void)word_type;
	char	*result;
	char	*exp;
	char	*tmp_result;
	int		i;
	int		start;
	char	quote;

	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			quote = str[i];
			start = ++i;
			while (str[i] && str[i] != quote)
				i++;
			if (quote == '\'')
				exp = ft_substr(str, start, i - start); // Pas d'expansion
			else
				exp = expand_token_value(ft_substr(str, start, i - start), env, last_status, WORD_DOUBLE_QUOTED);
			tmp_result = ft_strjoin(result, exp);
			free(result);
			result = tmp_result;
			free(exp);
			if (str[i] == quote)
				i++;
			continue;
		}
		if (str[i] == '$')
		{
			int var_start = i;
			i++;
			if (str[i] == '?')
			{
				i++;
				exp = ft_itoa(last_status);
			}
			else
			{
				while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
					i++;
				exp = ft_substr(str, var_start + 1, i - var_start - 1);
				exp = expand_var(exp, env, last_status);
			}
			tmp_result = ft_strjoin(result, exp);
			free(result);
			result = tmp_result;
			free(exp);
			continue;
		}
		start = i;
		while (str[i] && str[i] != '\'' && str[i] != '"' && str[i] != '$')
			i++;
		if (i > start)
		{
			exp = ft_substr(str, start, i - start);
			tmp_result = ft_strjoin(result, exp);
			free(result);
			result = tmp_result;
			free(exp);
		}
	}
	return (result);
}

