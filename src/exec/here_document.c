/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:46:46 by ebella            #+#    #+#             */
/*   Updated: 2025/06/17 17:17:25 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void heredoc_sigint(int sig)
{
	(void)sig;
	char c = '\n';

	g_signal_status = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	ioctl(STDIN_FILENO, TIOCSTI, &c);
}

int here_doc(const char *delim, int heredoc_expand, t_env *env, t_command *cmds)
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
			break ;
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
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	if (g_signal_status == 130)
	{
		cmds->status = 130;
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}

int prepare_heredocs(t_command *cmds, t_env *env)
{
    t_command *commands;
    t_redir *redirect;
    int fd;

	commands = cmds;
    while (commands)
    {
        redirect = commands->redir;
        while (redirect)
        {
            if (redirect->type == TOKEN_HEREDOC)
            {
                fd = here_doc(redirect->file, redirect->heredoc_expand, env, cmds);
                if (fd == -1)
                    return (0);
                redirect->heredoc_fd = fd;
            }
            redirect = redirect->next;
        }
        commands = commands->next;
    }
    return (1);
}