/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 11:22:04 by ebella            #+#    #+#             */
/*   Updated: 2025/05/30 11:55:17 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_unset(t_env *env, char *name)
{
	if (!env)
		return;
	while (env)
	{
		if (!ft_strcmp(name, env->name))
		{
			env->name = NULL;
			env->value = NULL;
		}	
		env = env->next;
	}
}
