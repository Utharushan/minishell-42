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

static t_redir	*create_redir(int type, char *file, int heredoc_expand)
{
	t_redir	*new;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (NULL);
	new->file = ft_strdup(file);
	if (!new->file)
	{
		free(new);
		return (NULL);
	}
	new->type = type;
	new->heredoc_expand = heredoc_expand;
	new->heredoc_fd = -1;
	new->next = NULL;
	return (new);
}

int	add_redir(t_command *cmd, int type, char *file, int heredoc_expand)
{
	t_redir	*new;
	t_redir	*tmp;

	new = create_redir(type, file, heredoc_expand);
	if (!new)
		return (0);
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
