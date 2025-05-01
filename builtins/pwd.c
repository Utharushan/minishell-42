/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:20:50 by ebella            #+#    #+#             */
/*   Updated: 2025/04/30 20:14:07 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd)
	{
		ft_putstr_fd(cwd, 1);
		ft_putstr_fd("\n", 1);
		free(cwd);
		return (0);
	}
	else
	{
		perror("pwd");
		return (1);
	}
}
