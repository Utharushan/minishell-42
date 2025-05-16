/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:39:04 by ebella            #+#    #+#             */
/*   Updated: 2025/05/16 12:04:03 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

static int	create_pipe(t_command *cmds, int *pipe_fd)
{
	if (cmds->next_op == OP_PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			ft_exit(cmds->args, cmds);
		}
		return (1);
	}
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	return (0);
}

static int		create_child_process(void)
{
	pid_t pid;
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	return (pid);
}

static void	handle_parent_process(t_command *cmds, int *in_fd, int *pipe_fd, pid_t pid)
{
	int status;
	
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		cmds->status = WEXITSTATUS(status);
	if (*in_fd != 0)
		close(*in_fd);
	if (cmds->next_op == OP_PIPE)
	{
		close(pipe_fd[1]);
		*in_fd = pipe_fd[0];
	}
}
static void	handle_child_process(t_command *cmds, char **envp, int in_fd, int *pipe_fd)
{
	if (in_fd != 0)
	{
		dup2(in_fd, STDIN_FILENO);
		close(in_fd);
	}
	if (cmds->next_op == OP_PIPE)
	{
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[0]);
		close(pipe_fd[1]);
	}
	if (!is_builtins(cmds))
	{
		if (!cmds->path)
			init_command_path(cmds, envp);
		if (find_cmd_in_path(cmds))
		{
			ft_putstr_fd(cmds->args[0], 2);
			ft_putstr_fd(": command not found\n", 2);
			cmds->status = 127;
		}else
		{
			exec_command(cmds, envp);
			exit(127);
		}
	}
	exit(cmds->status);
}
void	run_pipe(t_command *cmds, char **envp)
{
	int pipe_fd[2];
	int	in_fd;
	pid_t pid;
	
	in_fd = 0;
	while (cmds)
	{
		if (!ft_strncmp(cmds->args[0], "exit", 5))
		{
			if (cmds->next_op != OP_PIPE)
				ft_exit(cmds->args, cmds);
		}
		create_pipe(cmds, pipe_fd);
		pid = create_child_process();
		if (pid == 0)
			handle_child_process(cmds, envp, in_fd, pipe_fd);
		else
			handle_parent_process(cmds, &in_fd, pipe_fd, pid);
		cmds = cmds->next;
	}
}