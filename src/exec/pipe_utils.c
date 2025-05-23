/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:46:46 by ebella            #+#    #+#             */
/*   Updated: 2025/05/23 11:30:02 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int		count_cmds(t_command *cmds)
{
	int i;

	if (!cmds)
		return (0);
	i = 0;
	while (cmds)
	{
		i++;
		cmds = cmds->next;
	}
	return (i);
}

void	close_fd(int *in_fd, t_command *cmds, int *pipe_fd)
{
	if (*in_fd != 0)
		close(*in_fd);
	if (cmds->next_op == OP_PIPE)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
}