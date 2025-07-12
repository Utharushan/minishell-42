/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipes_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:28:03 by ebella            #+#    #+#             */
/*   Updated: 2025/07/12 12:48:31 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	create_pipe(t_command *cmds, int *pipe_fd)
{
	if (cmds->next_op == OP_PIPE)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe");
			exit(1);
		}
		return (1);
	}
	pipe_fd[0] = -1;
	pipe_fd[1] = -1;
	return (0);
}

int	create_child_process(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		exit(1);
	}
	return (pid);
}

static void	handle_child_exit_status(int status, t_command *cmds)
{
	int	exit_code;

	if (WIFEXITED(status))
	{
		exit_code = WEXITSTATUS(status);
		if (!cmds->next)
			singleton(1, exit_code);
	}
	else if (WIFSIGNALED(status))
		handle_signal_termination(status);
}

void	wait_for_pids(t_command *cmds, pid_t *pid)
{
	int	i;
	int	status;

	i = 0;
	while (cmds)
	{
		waitpid(pid[i], &status, 0);
		handle_child_exit_status(status, cmds);
		cmds = cmds->next;
		i++;
	}
}
