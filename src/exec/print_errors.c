/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 11:13:03 by ebella            #+#    #+#             */
/*   Updated: 2025/05/20 11:15:03 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	print_error(t_command *cmds)
{
	ft_putstr_fd(cmds->args[0], 2);
	ft_putstr_fd(": command not found\n", 2);
	cmds->status = 127;
}
