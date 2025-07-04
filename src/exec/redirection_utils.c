/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:20:13 by ebella            #+#    #+#             */
/*   Updated: 2025/07/04 21:20:55 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_redir	*find_last_heredoc(t_redir *redir)
{
	t_redir	*last;

	last = NULL;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
			last = redir;
		redir = redir->next;
	}
	return (last);
}

int	handle_redir_in(t_redir *redir)
{
	if (redirect_input(redir->file) == 0)
		return (0);
	return (1);
}

int	handle_redir_out(t_redir *redir)
{
	if (redirect_output(redir->file, false) == 0)
		return (0);
	return (1);
}

int	handle_redir_append(t_redir *redir)
{
	if (redirect_output(redir->file, true) == 0)
		return (0);
	return (1);
}
