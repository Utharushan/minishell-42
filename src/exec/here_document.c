/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:46:46 by ebella            #+#    #+#             */
/*   Updated: 2025/05/19 14:24:18 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int here_doc(const char *delim)
{
    int pipe_fd[2];
    char *line;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (0);
    }
    while (1)
    {
        line = readline(">");
        if (!line || ft_strncmp(line, delim, ft_strlen(delim)))
        {
            free(line);
            break;
        }
        ft_putstr_fd(line, pipe_fd[1]);
        ft_putstr_fd("\n", pipe_fd[1]);
        free(line);
    }
    close(pipe_fd[1]);
    return (pipe_fd[0]);
}
