/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 19:05:47 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/13 13:05:56 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*expand_token_value(const char *str, t_word_type word_type,
	t_env *env, int last_status)
{
	char	*result;
	int		len;
	char	*tmp;

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
			tmp = ft_substr(str, 1, len - 2);
			result = expand_token_value_unquoted(tmp, env, last_status);
			free(tmp);
			return (result);
		}
		return (expand_token_value_unquoted(str, env, last_status));
	}
	return (expand_token_value_unquoted(str, env, last_status));
}

static char	*append_char(const char *result, char c)
{
	char	tmp[2];
	char	*tmp_result;

	tmp[0] = c;
	tmp[1] = '\0';
	tmp_result = ft_strjoin(result, tmp);
	free((char *)result);
	return (tmp_result);
}

static char	*handle_escaped_dollar(const char *result, int *i)
{
	(*i) += 2;
	return (append_char(result, '$'));
}

static char	*expand_token_value_unquoted_loop(const char *str, t_env *env,
	int last_status, char *result)
{
	int		i;
	char	*exp;
	char	*tmp;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\\' && str[i + 1] == '$')
		{
			result = handle_escaped_dollar(result, &i);
			continue ;
		}
		if (str[i] == '$')
		{
			exp = expand_var(str, &i, env, last_status);
			tmp = result;
			result = ft_strjoin(result, exp);
			free(tmp);
			free(exp);
			continue ;
		}
		result = append_char(result, str[i]);
		i++;
	}
	return (result);
}

char	*expand_token_value_unquoted(const char *str, t_env *env,
	int last_status)
{
	char	*result;

	result = ft_strdup("");
	result = expand_token_value_unquoted_loop(str, env, last_status, result);
	return (result);
}
