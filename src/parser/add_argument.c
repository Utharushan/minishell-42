/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_argument.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:26:44 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/11 21:26:44 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	**grow_args(char **old_args, char *new_arg, int count)
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
	new_args[count] = new_arg;
	new_args[count + 1] = NULL;
	return (new_args);
}

int	add_argument(t_command *cmd, char *arg, t_word_type word_type, t_env *env)
{
	int		count;
	char	**new_args;
	char	*final_arg;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	final_arg = expand_token_value(arg, word_type, env, singleton(0, 0));
	if (!final_arg)
		return (0);
	new_args = grow_args(cmd->args, final_arg, count);
	if (!new_args)
	{
		free(final_arg);
		return (0);
	}
	free(cmd->args);
	cmd->args = new_args;
	return (1);
}
