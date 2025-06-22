/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:06:55 by ebella            #+#    #+#             */
/*   Updated: 2025/06/22 15:17:57 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_echo(char **args, t_env *env)
{
	int	i;
	int	n_flag;

	(void)env;
	i = 1;
	n_flag = 0;
	
	// Check for -n flag
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
	}
	
	// Print arguments
	while (args[i])
	{
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
		}
		i++;
	}
	
	// Add newline if -n flag is not present
	if (!n_flag)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
	}
	
	return (0);
}
