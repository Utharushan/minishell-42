/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/18 14:40:00 by ebella            #+#    #+#             */
/*   Updated: 2025/05/19 14:18:26 by ebella           ###   ########.fr       */
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
        fd = open(outfile, O_APPEND | O_CREAT | O_WRONLY, 0777);
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

int command_redirections(t_command *cmd)
{
    int fd;

    if (cmd->input)
    {
        fd = open(cmd->input, O_RDONLY);
        if (fd == -1)
        {
            perror(cmd->input);
            cmd->status = 1;
            exit(cmd->status);
        }
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    if (cmd->input)
    {
        if (redirect_input(cmd->input) == 0)
            return (0);
    }
    if (cmd->output)
    {
        if (redirect_output(cmd->output, cmd->append) == 0)
            return (0);
    }
    if (cmd->here_doc == TOKEN_HEREDOC)
    {
        fd = here_doc(cmd->args[1]); // j'ai essayer ca, mais je pense que le parsing doit etre refait. Quand on detect << tu met pas le deuxieme arguemt dans args[1], pcq sinon ca va essayer de le lancer comme une cmd, au lieu de juste le mettre dans ma fonction comme delim.
        if (fd == -1)
            return (0);
        dup2(fd, STDIN_FILENO);
        close(fd);
    }
    return (1);
}
