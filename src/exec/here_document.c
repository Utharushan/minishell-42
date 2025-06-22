/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_document.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/19 13:46:46 by ebella            #+#    #+#             */
/*   Updated: 2025/06/22 12:25:58 by tuthayak         ###   ########.fr       */
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

void setup_heredoc_signals(void)
{
    signal(SIGINT, heredoc_sigint);
    signal(SIGQUIT, SIG_IGN);
    g_signal_status = 0;
}

void restore_heredoc_signals(void)
{
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
}

int heredoc_write_line(char *line, int heredoc_expand, t_env *env, int fd)
{
    char *expanded;

    if (heredoc_expand)
    {
        expanded = expand_token_value((char *)line, env, g_signal_status, WORD_UNQUOTED);
        ft_putstr_fd(expanded, fd);
        free(expanded);
    }
    else
        ft_putstr_fd(line, fd);
    ft_putchar_fd('\n', fd);
    return (0);
}

int heredoc_loop(const char *delim, int heredoc_expand, t_env *env, int fd)
{
    char *line;

    while (g_signal_status != 130)
    {
        line = readline("> ");
        if (g_signal_status == 130 || !line || !ft_strcmp(line, delim))
        {
            free(line);
            break;
        }
        heredoc_write_line(line, heredoc_expand, env, fd);
        free(line);
    }
    return (0);
}

// Ajout d'une fonction utilitaire pour détecter les quotes autour du délimiteur
int is_quoted_delim(const char *delim)
{
    int len = ft_strlen(delim);
    if ((delim[0] == '"' && delim[len-1] == '"') || (delim[0] == '\'' && delim[len-1] == '\''))
        return 1;
    return 0;
}

int here_doc(const char *delim, int heredoc_expand, t_env *env, t_minishell *mini)
{
    int pipe_fd[2];
    int expand;

    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        return (0);
    }
    // Expansion seulement si le délimiteur n'est pas entre quotes
    expand = heredoc_expand;
    if (is_quoted_delim(delim))
        expand = 0;
    setup_heredoc_signals();
    heredoc_loop(delim, expand, env, pipe_fd[1]);
    close(pipe_fd[1]);
    restore_heredoc_signals();
    if (g_signal_status == 130)
    {
        mini->status = 130;
        close(pipe_fd[0]);
        return (-1);
    }
    return (pipe_fd[0]);
}

int prepare_heredocs(t_command *cmds, t_env *env, t_minishell *mini)
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
                fd = here_doc(redirect->file, redirect->heredoc_expand, env, mini);
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