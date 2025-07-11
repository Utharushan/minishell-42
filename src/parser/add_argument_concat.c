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

static char	*concat_tokens(t_token **tokens, t_env *env)
{
	char	*arg;
	char	*tmp;
	char	*expanded;
	int		first;

	first = 1;
	arg = ft_strdup("");
	if (!arg)
		return (NULL);
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
	return (arg);
}

static char	**grow_args_concat(char **old_args, char *new_arg, int count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	while (i < count)
	{
		new_args[i] = old_args[i];
		i++;
	}
	new_args[count] = ft_strdup(new_arg);
	new_args[count + 1] = NULL;
	return (new_args);
}

int	add_argument_concat(t_command *cmd, t_token **tokens, t_env *env)
{
	int		count;
	char	*arg;
	char	**new_args;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	arg = concat_tokens(tokens, env);
	if (!arg)
		return (0);
	new_args = grow_args_concat(cmd->args, arg, count);
	if (!new_args)
	{
		free(arg);
		return (0);
	}
	free(cmd->args);
	cmd->args = new_args;
	free(arg);
	return (1);
}
