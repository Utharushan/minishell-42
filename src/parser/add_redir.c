/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_redir.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:26:50 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/11 21:26:50 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	add_redir(t_command *cmd, int type, char *file, int heredoc_expand)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (0);
	new->file = ft_strdup(file);
	if (!new->file)
	{
		free(new);
		return (0);
	}
	new->type = type;
	new->heredoc_expand = heredoc_expand;
	new->heredoc_fd = -1;
	new->next = NULL;
	if (!cmd->redir)
		cmd->redir = new;
	else
	{
		tmp = cmd->redir;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
	return (1);
}
