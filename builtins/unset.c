/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:22:04 by ebella            #+#    #+#             */
/*   Updated: 2025/05/30 21:33:50 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_unset(t_env **env, char *name)
{
	t_env *cur = *env;
	t_env *prev = NULL;
	t_env *tmp;

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
			free(tmp->name);
			free(tmp->value);
			free(tmp);
			return ;
		}
		else
		{
			prev = cur;
			cur = cur->next;
		}
	}
}


