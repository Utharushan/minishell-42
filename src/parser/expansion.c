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
	if ((!val || !*val) && name && ft_strcmp(name, "PWD") == 0)
	{
		char cwd[4096];
		if (getcwd(cwd, sizeof(cwd)))
		{
			char *pwd_str = ft_strjoin("PWD=", cwd);
			add_new_node(env, pwd_str);
			free(pwd_str);
			free(name);
			return (ft_strdup(cwd));
		}
	}
	free(name);
	if (val)
		return (ft_strdup(val));
	else
		return (ft_strdup(""));
}

char	*expand_token_value(const char *str, t_word_type word_type, t_env *env, int last_status)
{
	char	*result;
	int		len;

	if (!str)
		return (NULL);
	len = ft_strlen(str);
	if (word_type == WORD_SINGLE_QUOTED)
	{
		if (len >= 2 && str[0] == '\'' && str[len - 1] == '\'')
			return (ft_substr(str, 1, len - 2));
		return (ft_strdup(str));
	}
	if (word_type == WORD_DOUBLE_QUOTED)
	{
		if (len >= 2 && str[0] == '"' && str[len - 1] == '"')
		{
			char	*tmp = ft_substr(str, 1, len - 2);
			result = expand_token_value_unquoted(tmp, env, last_status);
			free(tmp);
			return (result);
		}
		return (expand_token_value_unquoted(str, env, last_status));
	}
	return (expand_token_value_unquoted(str, env, last_status));
}

char	*expand_token_value_unquoted(const char *str, t_env *env, int last_status)
{
	char	*result;
	int		i = 0;
	int		len = ft_strlen(str);
	char	*exp;
	char	tmp[2];
	char	*tmp_result;

	result = ft_strdup("");
	while (i < len)
	{
		if (str[i] == '\\' && (i + 1 < len) && str[i + 1] == '$')
		{
			tmp[0] = '$';
			tmp[1] = '\0';
			tmp_result = ft_strjoin(result, tmp);
			free(result);
			result = tmp_result;
			i += 2;
			continue;
		}
		if (str[i] == '$')
		{
			exp = expand_var(str, &i, env, last_status);
			tmp_result = ft_strjoin(result, exp);
			free(result);
			result = tmp_result;
			free(exp);
			continue;
		}
		tmp[0] = str[i];
		tmp[1] = '\0';
		tmp_result = ft_strjoin(result, tmp);
		free(result);
		result = tmp_result;
		i++;
	}
	return (result);
}

