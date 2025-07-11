/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_argument_concat.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:26:40 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/11 21:26:40 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_argument_concat(t_command *cmd, t_token **tokens, t_env *env)
{
	char	*arg;
	char	*tmp;
	int		first;
	int		count;
	int		i;
	char	**new_args;
	char	*expanded;

	first = 1;
	arg = ft_strdup("");
	if (!arg)
		return (0);
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		if (!first && (*tokens)->has_leading_space)
			break ;
		expanded = expand_token_value((*tokens)->value,
				(*tokens)->word_type, env, singleton(0, 0));
		tmp = arg;
		arg = ft_strjoin(arg, expanded);
		free(tmp);
		free(expanded);
		*tokens = (*tokens)->next;
		first = 0;
	}
	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
	{
		free(arg);
		return (0);
	}
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	free(arg);
	return (1);
}
