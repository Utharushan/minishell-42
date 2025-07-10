/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 22:09:57 by ebella            #+#    #+#             */
/*   Updated: 2025/07/10 22:12:14 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	parse_exit_args(char **args, t_info *info)
{
	int	exit_code;

	if (!args || !args[1])
		return (singleton(0, 0));
	if (!is_numeric(args[1]))
	{
		ft_putstr_fd("exit: numeric argument required\n", 2);
		singleton(1, 2);
		free_info_and_exit(info, singleton(0, 0));
	}
	if (args[2])
	{
		ft_putstr_fd("exit: too many arguments\n", 2);
		singleton(1, 1);
		return (-1);
	}
	exit_code = ft_atoi(args[1]) % 256;
	return (exit_code);
}
