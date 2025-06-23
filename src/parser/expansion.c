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

char	*expand_token_value(char *token_value, t_env *env, t_word_type word_type)
{
	char	*result;
	int		i;
	int		j;

	if (!token_value)
		return (NULL);
	
	// No expansion for single quoted words
	if (word_type == WORD_SINGLE_QUOTED)
		return (ft_strdup(token_value));
	
	result = malloc(sizeof(char) * (ft_strlen(token_value) * 10 + 1));
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	
	while (token_value[i])
	{
		if (token_value[i] == '$' && word_type != WORD_SINGLE_QUOTED)
		{
			i++;
			if (token_value[i] == '?')
			{
				char *exit_status = ft_itoa(g_exit_status);
				if (exit_status)
				{
					ft_strlcpy(result + j, exit_status, ft_strlen(exit_status) + 1);
					j += ft_strlen(exit_status);
					free(exit_status);
				}
				i++;
			}
			else if (ft_isalpha(token_value[i]) || token_value[i] == '_')
			{
				int var_start = i;
				while (token_value[i] && (ft_isalnum(token_value[i]) || token_value[i] == '_'))
					i++;
				char *var_name = ft_substr(token_value, var_start, i - var_start);
				char *var_value = ft_getenv(env, var_name);
				if (var_value)
				{
					ft_strlcpy(result + j, var_value, ft_strlen(var_value) + 1);
					j += ft_strlen(var_value);
				}
				free(var_name);
			}
			else
			{
				result[j++] = '$';
			}
		}
		else
		{
			result[j++] = token_value[i++];
		}
	}
	
	result[j] = '\0';
	return (result);
}

