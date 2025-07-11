/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pipe.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 21:26:56 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/11 21:26:56 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

t_command	*handle_pipe(t_command *cmd)
{
	cmd->next_op = OP_PIPE;
	cmd->next = new_command();
	return (cmd->next);
}
