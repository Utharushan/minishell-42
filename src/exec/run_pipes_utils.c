/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipes_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:28:03 by ebella            #+#    #+#             */
/*   Updated: 2025/07/05 21:18:29 by ebella           ###   ########.fr       */
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
The function is called in the parent process after forking a child.

Wait for the child process to end, register its status.

If in_fd is not stdin (0), close it.
If this command is followed by a pipe, we close its write end,
and save the read end into in_fd, so we can use it as the input
for the next command.

it connects the command through the pipes.
*/
void	wait_for_pids(t_command *cmds, pid_t *pid)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (cmds)
	{
		waitpid(pid[i], &status, 0);
		if (WIFEXITED(status))
			singleton(1, WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
		{
			if (WTERMSIG(status) == SIGINT)
				g_signal_status = 130;
			else if (WTERMSIG(status) == SIGQUIT)
			{
				g_signal_status = 131;
				ft_putstr_fd("Quit (core dumped)\n", 2);
			}
		}
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
        {
            printf("ici\n");
            exit(singleton(0, 0));
        }
			
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
