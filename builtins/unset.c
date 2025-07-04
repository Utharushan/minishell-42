/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:22:04 by ebella            #+#    #+#             */
/*   Updated: 2025/06/23 19:54:00 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	free_tmp(t_env *tmp)
{
	free(tmp->name);
	free(tmp->value);
	free(tmp);
}
int		ft_unset(t_env **env, char *name)
{
	t_env	*cur;
	t_env	*prev;
	t_env	*tmp;

	if (!name)
		return (0);
	cur = *env;
	prev = NULL;
	while (cur)
	{
		if (!ft_strcmp(cur->name, name))
		{
			tmp = cur;
			if (!prev)
				*env = cur->next;
			else
				prev->next = cur->next;
			cur = cur->next;
			free_tmp(tmp);
			return (0);
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
	return (0);
}
