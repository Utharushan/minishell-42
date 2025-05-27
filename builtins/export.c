/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 13:09:43 by ebella            #+#    #+#             */
/*   Updated: 2025/05/27 10:33:13 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_export(t_env *env, char *str)
{
	int i;
	char	*name;
	char	*value;
	char	*equal_position;	
	t_env	*new_env;

	i = 0;
	if (!env || !str)
		return (1);
	equal_position = ft_strchr(str, '=');
	if (equal_position)
	{
		name = ft_substr(str, 0, equal_position - str);
		value = ft_strdup(equal_position);
		new_env = new_env_node(name, value);
		add_env_back(&env, new_env);
	}
	return (0);
}
