/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:22:04 by ebella            #+#    #+#             */
/*   Updated: 2025/07/13 13:52:16 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tmp(t_env *tmp)
{
	free(tmp->name);
	free(tmp->value);
	free(tmp);
}

int		remove_env_node(t_env **env, t_env *prev, t_env *cur)
{
	t_env	*tmp;

	tmp = cur;
	if (!prev)
		*env = cur->next;
	else
		prev->next = cur->next;
	free_tmp(tmp);
	return (1);
}

int	ft_unset(t_env **env, char **name)
{
	t_env	*cur;
	t_env	*prev;
	int		i;

	if (!name)
		return (0);
	i = 0;
	while (name[++i])
	{
		cur = *env;
		prev = NULL;
		while (cur && cur->name)
		{
				if (!ft_strcmp(cur->name, name[i]) && remove_env_node(env, prev, cur))
					break;
				else
				{
					prev = cur;
					cur = cur->next;
				}
		}
	}
	return (0);
}
