/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:43:37 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/05 21:13:45 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_redir(t_redir *redir)
{
	t_redir *tmp;

	while (redir)
	{
		tmp = redir;
		if (redir->file)
			free(redir->file);
		redir = redir->next;
		free(tmp);
	}
}

void free_args(t_command *cmd)
{
	int i;

	i = 0;
	while (cmd->args[i])
		free(cmd->args[i++]);
	free(cmd->args);
}

void free_command_list(t_command *cmd)
{
	t_command *tmp;

	if (!cmd)
		return ;
	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
			free_args(cmd);
		if (cmd->input)
			free(cmd->input);
		if (cmd->output)
			free(cmd->output);
		if (cmd->heredoc_delim)
			free(cmd->heredoc_delim);
		if (cmd->redir)
		{
			close_all_heredoc_fds(cmd);
			free_redir(cmd->redir);
			cmd->redir = NULL;
		}
		free(cmd);
		cmd = tmp;
	}
}

int is_numeric(const char *str)
{
	if (!str || *str == '\0')
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

void ft_exit(char **args, t_info *info)
{
	int exit_code;

    exit_code = 0;
	if (args && args[1])
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			singleton(1, 2);
			free_info_and_exit(info,singleton(0, 0));
		}
		else if (args[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			singleton(1, 1);
			return;
		}
		else
			exit_code = ft_atoi(args[1]) % 256;
	}
	else
		exit_code = singleton(0, 0);
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 1);
	if (info->env)
		free_env_list(info->env);
	free_info_child(info);
	exit(exit_code);
}
