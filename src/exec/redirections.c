/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:40:00 by ebella            #+#    #+#             */
/*   Updated: 2025/06/11 13:43:50 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int redirect_input(char *infile)
{
	int fd;

	fd = open(infile, O_RDONLY);
	if (fd < 0)
	{
		perror(infile);
		return (0);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int redirect_output(char *outfile, bool append)
{
	int fd;

	if (append)
		fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0777);
	else
		fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0777);
	if (fd < 0)
	{
		perror(outfile);
		return (0);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (0);
	}
	close(fd);
	return (1);
}

int	command_redirections(t_command *cmd)
{
    t_redir	*redir;
    t_redir	*last_heredoc;

    redir = cmd->redir;
    last_heredoc = NULL;
    while (redir)
    {
        if (redir->type == TOKEN_HEREDOC)
            last_heredoc = redir;
        redir = redir->next;
    }
    redir = cmd->redir;
    while (redir)
    {
        if (redir->type == TOKEN_REDIRECT_IN)
        {
            if (redirect_input(redir->file) == 0)
                return (0);
        }
        else if (redir->type == TOKEN_REDIRECT_OUT)
        {
            if (redirect_output(redir->file, false) == 0)
                return (0);
        }
        else if (redir->type == TOKEN_REDIRECT_APPEND)
        {
            if (redirect_output(redir->file, true) == 0)
                return (0);
        }
        else if (redir->type == TOKEN_HEREDOC)
        {
            if (redir == last_heredoc)
            {
                if (dup2(redir->heredoc_fd, STDIN_FILENO) == -1)
                {
                    perror("dup2");
                    close(redir->heredoc_fd);
                    return (0);
                }
            }
            close(redir->heredoc_fd);
        }
        redir = redir->next;
    }
    return (1);
}
