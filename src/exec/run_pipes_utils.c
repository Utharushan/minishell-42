/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipes_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:28:03 by ebella            #+#    #+#             */
/*   Updated: 2025/07/06 14:36:22 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
If the current command is followed by a pipe (cmds->next_op == OP_PIPE),
this function creates a pipe.
The	pipe(void) function stores the read and write file descriptors in pipe_fd.

If pipe creation fails, it prints an error message and exits.

If no pipe is needed, it sets pipe_fd to -1 on both ends to indicate
that no pipe was created.

Returns 1 if a pipe was created, 0 otherwise.
*/
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

/*
Create a child process by using fork().
Returns the process ID, so we can know if we are in a child process.
*/
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

/*
Handle the exit status of a child process.
If the process exited normally, store the exit code.
If it was killed by a signal, set the appropriate global signal status.
*/
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

/*
Wait for all child processes to complete and handle their exit status.
For pipelines, only the exit code of the last command is preserved.
*/
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

/*
Check's if in_fd is not equal to zero, that means that we need to redirect stdin
into our pipe in_fd,
	so the command read's from the pipe instead of the terminal.
close in_fd because we dont need it anymore.

Next if the next op is a pipe, we change our stdout to go into our pipe.
we close our pipe_fd[0] and pipe_fd[1], so we dont get fd leaks.

Next we check if its a builtin, and if the command exist.
If everything is good we execute it.

and we exit with the current exit code.
*/
pid_t	*init_pipe_exec(t_command *cmds, t_env *env)
{
	pid_t	*pid;
	t_info	*info;

	if (cmds && !cmds->next && cmds->args && !ft_strncmp(cmds->args[0], "exit",
			5))
	{
		info = init_info(cmds, env);
		if (info)
			ft_exit(cmds->args, info);
		else
			exit(singleton(0, 0));
	}
	pid = malloc(sizeof(pid_t) * count_cmds(cmds));
	if (!pid)
		return (NULL);
	if (!cmds)
	{
		free(pid);
		return (NULL);
	}
	return (pid);
}
