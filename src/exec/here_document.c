/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:46:46 by ebella            #+#    #+#             */
/*   Updated: 2025/06/05 18:28:12 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void heredoc_sigint(int sig)
{
	(void)sig;
	g_signal_status = 130;
	write(1, "\n", 1);
}

int here_doc(const char *delim, int heredoc_expand, t_env *env)
{
	int pipe_fd[2];
	char *line;
	char *expanded;

	if (pipe(pipe_fd) == -1)
	{
		perror("pipe");
		return (0);
	}
	signal(SIGINT, heredoc_sigint);
	signal(SIGQUIT, SIG_IGN);
	g_signal_status = 0;
	while (g_signal_status != 130)
	{
		line = readline("> ");
		if (g_signal_status == 130 || !line || !ft_strcmp(line, delim))
		{
			free(line);
			break;
		}
		if (heredoc_expand)
		{
			expanded = expand_token_value(line, env, g_signal_status);
			ft_putstr_fd(expanded, pipe_fd[1]);
			free(expanded);
		}
		else
			ft_putstr_fd(line, pipe_fd[1]);
		ft_putchar_fd('\n', pipe_fd[1]);
		free(line);
	}
	close(pipe_fd[1]);
	signal(SIGINT, SIG_DFL);
	if (g_signal_status == 130)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}
