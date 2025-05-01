/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:06:55 by ebella            #+#    #+#             */
/*   Updated: 2025/04/30 13:43:32 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	check_args(char **args)
{
	int	i;

	i = 1;
	if (args[0] != '-' || args[1] != 'n')
		return (0);
	while (args[i])
	{
		if (args[i] != 'n')
			return (0);
		i++;
	}
	return (1);
}

void     ft_echo(t_command *cmds)
{
	int	i;
	int	n_flag;

    i = 1;
    n_flag = 0;
    while (cmds->args[i] && check_args(cmds->args[i]))
    {
        n_flag = 1;
        i++;
    }
    while (cmds->args[i])
    {
        ft_putstr_fd(cmds->args[i], 1);
        if (cmds->args[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (!n_flag)
        ft_putstr_fd("\n", 1);
}
