/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 13:43:37 by tuthayak          #+#    #+#             */
/*   Updated: 2025/05/30 14:15:06 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void free_command(t_command *cmd)
{
	int i = 0;

	if (!cmd)
		return;
	if (cmd->args)
	{
		while (cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
	}
	i = 0;
	if (cmd->path)
	{
		while (cmd->path[i])
			free(cmd->path[i++]);
		free(cmd->path);
	}
	free(cmd);
}

int free_struct(t_command *cmds)
{
	t_command *tmp;

	while (cmds)
	{
		tmp = cmds->next;
		free_command(cmds);
		cmds = tmp;
	}
	return (0);
}

void	free_command_list(t_command *cmd)
{
	t_command *tmp;
	int i;

	while (cmd)
	{
		tmp = cmd->next;
		if (cmd->args)
		{
			i = 0;
			while (cmd->args[i])
			{
				free(cmd->args[i]);
				i++;
			}
			free(cmd->args);
		}
		if (cmd->input)
			free(cmd->input);
		if (cmd->output)
			free(cmd->output);
		if (cmd->heredoc_delim)
			free(cmd->heredoc_delim);
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

void ft_exit(char **args, t_command *cmds)
{
	int exit_code;

	if (args && args[1])
	{
		if (!is_numeric(args[1]))
		{
			ft_putstr_fd("exit: numeric argument required\n", 2);
			cmds->status = 2;
			exit(cmds->status);
		}
		else if (args[2])
		{
			ft_putstr_fd("exit: too many arguments\n", 2);
			cmds->status = 1;
			return;
		}
		else
			exit_code = ft_atoi(args[1]) % 256;
	}
	else
		exit_code = cmds->status;
	if (isatty(STDIN_FILENO))
		ft_putstr_fd("exit\n", 1);
	exit(exit_code);
}
