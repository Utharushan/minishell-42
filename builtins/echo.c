/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 17:06:55 by ebella            #+#    #+#             */
/*   Updated: 2025/03/25 17:15:06 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static  int     count_args(char **args)
{
    int i;

    i = 0;
    while (args[i])
        i++;
    return (i);
}

int     echo(char **args)
{
    int i;
    int n_flag;

    i = 1;
    n_flag = 0;
    if (count_args(args) > 1 && !ft_strcmp(args[1], "-n"))
    {
        n_flag = 1;
        i++;
    }
    while (args[i])
    {
        ft_putstr_fd(args[i], 1);
        if (args[i + 1])
            ft_putstr_fd(" ", 1);
        i++;
    }
    if (!n_flag)
        ft_putstr_fd("\n", 1);
    return (0);
}
