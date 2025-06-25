/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:40:00 by ebella            #+#    #+#             */
/*   Updated: 2025/06/25 14:55:32 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	redirect_input(char *infile)
{
	int	fd;

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

int	redirect_output(char *outfile, bool append)
{
	int	fd;

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

t_redir	*find_last_heredoc(t_redir *redir)
{
	t_redir	*last;

	last = NULL;
	while (redir)
	{
		if (redir->type == TOKEN_HEREDOC)
			last = redir;
		redir = redir->next;
	}
	return (last);
}

int	handle_redir_in(t_redir *redir)
{
	if (redirect_input(redir->file) == 0)
		return (0);
	return (1);
}

int	handle_redir_out(t_redir *redir)
{
	if (redirect_output(redir->file, false) == 0)
		return (0);
	return (1);
}

int	handle_redir_append(t_redir *redir)
{
	if (redirect_output(redir->file, true) == 0)
		return (0);
	return (1);
}

int	handle_heredoc(t_redir *redir, t_redir *last_heredoc)
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
	return (1);
}

int	handle_one_redir(t_redir *redir, t_redir *last_heredoc)
{
	if (redir->type == TOKEN_REDIRECT_IN)
		return (handle_redir_in(redir));
	else if (redir->type == TOKEN_REDIRECT_OUT)
		return (handle_redir_out(redir));
	else if (redir->type == TOKEN_REDIRECT_APPEND)
		return (handle_redir_append(redir));
	else if (redir->type == TOKEN_HEREDOC)
		return (handle_heredoc(redir, last_heredoc));
	return (1);
}

int	command_redirections(t_command *cmd)
{
	t_redir	*redir;
	t_redir	*last_heredoc;

	redir = cmd->redir;
	last_heredoc = find_last_heredoc(cmd->redir);
	while (redir)
	{
		if (!handle_one_redir(redir, last_heredoc))
			return (0);
		redir = redir->next;
	}
	return (1);
}
