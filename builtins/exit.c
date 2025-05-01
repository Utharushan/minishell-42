/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:43:37 by tuthayak          #+#    #+#             */
/*   Updated: 2025/04/30 13:46:11 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		free_struct(t_command *cmds)
{
	t_command *tmp;
	int			i;

	i = 0;
	if (!cmds)
		return (1);
	while (cmds)
	{
		tmp = cmds;
		while (cmds->args[i])
			free(cmds->args[i++]);
		i = 0;
		while (cmds->path[i])
			free(cmds->path[i++]);
		free(cmds);
		cmds = tmp->next;
	}
	return (0);
}
int	ft_exit(int status,	t_command	*cmds)
{
	free_struct(cmds);
	exit(status);
	return (0);
}