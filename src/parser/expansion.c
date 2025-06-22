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

char	*expand_token_value(char *token_value, t_env *env)
{
	char	*result;
	int		i;
	int		j;
	int		in_quotes;
	char	quote;

	if (!token_value)
		return (NULL);
	
	printf("DEBUG EXPANSION: Starting expansion of token: '%s'\n", token_value);
	
	result = malloc(sizeof(char) * (ft_strlen(token_value) * 10 + 1));
	if (!result)
		return (NULL);
	
	i = 0;
	j = 0;
	in_quotes = 0;
	quote = 0;
	
	while (token_value[i])
	{
		printf("DEBUG EXPANSION: Processing char '%c' at index %d, in_quotes=%d\n", token_value[i], i, in_quotes);
		
		if ((token_value[i] == '\'' || token_value[i] == '"') && !in_quotes)
		{
			quote = token_value[i];
			in_quotes = 1;
			printf("DEBUG EXPANSION: Entering quotes '%c'\n", quote);
			i++;
		}
		else if (token_value[i] == quote)
		{
			in_quotes = 0;
			quote = 0;
			printf("DEBUG EXPANSION: Exiting quotes\n");
			i++;
		}
		else if (token_value[i] == '$' && quote != '\'')
		{
			printf("DEBUG EXPANSION: Found $ at index %d\n", i);
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
				printf("DEBUG EXPANSION: Expanded $? to %d\n", g_exit_status);
			}
			else if (ft_isalpha(token_value[i]) || token_value[i] == '_')
			{
				int var_start = i;
				while (token_value[i] && (ft_isalnum(token_value[i]) || token_value[i] == '_'))
					i++;
				char *var_name = ft_substr(token_value, var_start, i - var_start);
				printf("DEBUG EXPANSION: Variable name: '%s'\n", var_name);
				char *var_value = ft_getenv(env, var_name);
				if (var_value)
				{
					ft_strlcpy(result + j, var_value, ft_strlen(var_value) + 1);
					j += ft_strlen(var_value);
					printf("DEBUG EXPANSION: Expanded %s to '%s'\n", var_name, var_value);
				}
				free(var_name);
			}
			else
			{
				result[j++] = '$';
				printf("DEBUG EXPANSION: Invalid variable, keeping $\n");
			}
		}
		else
		{
			result[j++] = token_value[i++];
			printf("DEBUG EXPANSION: Copied char '%c' to result[%d]\n", token_value[i-1], j-1);
		}
	}
	
	result[j] = '\0';
	printf("DEBUG EXPANSION: Final result: '%s' (length: %d)\n", result, j);
	return (result);
}

