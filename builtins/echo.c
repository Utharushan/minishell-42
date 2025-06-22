/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:06:55 by ebella            #+#    #+#             */
/*   Updated: 2025/06/22 13:17:19 by tuthayak         ###   ########.fr       */
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
	
	printf("DEBUG ECHO: Starting echo with args:\n");
	for (int j = 0; args[j]; j++)
		printf("DEBUG ECHO: args[%d] = '%s'\n", j, args[j]);
	
	// Check for -n flag
	if (args[i] && ft_strcmp(args[i], "-n") == 0)
	{
		n_flag = 1;
		i++;
		printf("DEBUG ECHO: Found -n flag\n");
	}
	
	// Print arguments
	while (args[i])
	{
		printf("DEBUG ECHO: Printing arg[%d]: '%s'\n", i, args[i]);
		ft_putstr_fd(args[i], STDOUT_FILENO);
		if (args[i + 1])
		{
			ft_putchar_fd(' ', STDOUT_FILENO);
			printf("DEBUG ECHO: Added space\n");
		}
		i++;
	}
	
	// Add newline if -n flag is not present
	if (!n_flag)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		printf("DEBUG ECHO: Added newline\n");
	}
	
	return (0);
}
