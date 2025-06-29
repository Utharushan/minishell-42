/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run_pipe.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 12:39:04 by ebella            #+#    #+#             */
/*   Updated: 2025/06/29 22:05:20 by ebella           ###   ########.fr       */
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
int create_pipe(t_command *cmds, int *pipe_fd)
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
int create_child_process(void)
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
/*
The function is called in the parent process after forking a child.

Wait for the child process to end, register its status.

If in_fd is not stdin (0), close it.
If this command is followed by a pipe, we close its write end,
and save the read end into in_fd, so we can use it as the input
for the next command.

it connects the command through the pipes.
*/
void wait_for_pids(t_command *cmds, pid_t *pid)
{
	int i;
	int status;

	i = 0;
	status = 0;
	while (cmds)
	{
		waitpid(pid[i], &status, 0);
		if (WIFEXITED(status))
			g_signal_status = WEXITSTATUS(status);
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

int is_parent_builtin(t_command *cmds)
{
	if (!cmds->args || !cmds->args[0])
		return (0);
	if (!ft_strncmp(cmds->args[0], "cd", 3))
		return (1);
	else if (!ft_strncmp(cmds->args[0], "export", 7))
		return (1);
	else if (!ft_strncmp(cmds->args[0], "unset", 6))
		return (1);
	else if (!ft_strncmp(cmds->args[0], "exit", 5))
		return (1);
	return (0);
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
void setup_child_fds(t_command *cmds, int in_fd, int *pipe_fd)
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
}

void exec_child_builtin(t_command *cmds, t_env *env, pid_t *pid,
						t_command *first_cmds)
{
	if (is_parent_builtin(cmds))
	{
		free_env_list(env);
		free_command_list(first_cmds);
		free(pid);
		exit(0);
	}
	else
	{
		run_builtins(cmds, env);
		free_env_list(env);
		free_command_list(first_cmds);
	}
	free(pid);
	exit(g_signal_status);
}

void exec_child_external(t_command *cmds, t_env *env, pid_t *pid,
						 t_command *first_cmds)
{
	if (!ft_strncmp(cmds->args[0], "exit", 5))
	{
		free_env_list(env);
		free_command_list(first_cmds);
		exit(g_signal_status);
	}
	exec_command(cmds, env);
	free_env_list(env);
	free_command_list(first_cmds);
	free(pid);
	exit(g_signal_status);
}

void handle_child_process(t_command *cmds, int in_fd, int *pipe_fd, t_env *env,
						  pid_t *pid, t_command *first_cmds)
{
	setup_child_fds(cmds, in_fd, pipe_fd);
	if (command_redirections(cmds) == 0)
	{
		free_env_list(env);
		free_command_list(first_cmds);
		free(pid);
		exit(130);
	}
	if (is_builtins(cmds) == 0)
		exec_child_builtin(cmds, env, pid, first_cmds);
	child_process_signals();
	exec_child_external(cmds, env, pid, first_cmds);
}

int handle_parent_builtin_if_needed(t_command *cmds, t_env *env, pid_t *pid)
{
	if (!cmds->next && is_parent_builtin(cmds))
	{
		run_builtins(cmds, env);
		free(pid);
		return (1);
	}
	return (0);
}

void launch_child_process(t_command *cmds, int *in_fd, int *pipe_fd,
						  pid_t *pid, int *i, t_env *env, t_command *first_cmds)
{
	create_pipe(cmds, pipe_fd);
	pid[*i] = create_child_process();
	if (pid[(*i)++] == 0)
		handle_child_process(cmds, *in_fd, pipe_fd, env, pid, first_cmds);
	else
		close_fd(in_fd, cmds, pipe_fd);
	close_fd(in_fd, cmds, pipe_fd);
}

/*
Iterates through each cmd in the linked list.

For each command we create a pipe if needed, and we fork a new child process.

Manages fd's at each step to make sure that the output of one
command becomes the input of the next in the pipe.
*/
void run_pipe(t_command *cmds, t_env *env)
{
	int pipe_fd[2];
	int in_fd;
	pid_t *pid;
	t_command *first_cmds;
	int i;

	in_fd = 0;
	first_cmds = cmds;
	i = 0;
	if (first_cmds && !first_cmds->next && first_cmds->args && !ft_strncmp(first_cmds->args[0], "exit", 5))
		ft_exit(first_cmds->args, cmds, env);
	pid = malloc(sizeof(pid_t) * count_cmds(cmds));
	if (!pid)
		return;
	if (!cmds)
	{
		free(pid);
		return;
	}
	while (cmds)
	{
		if (handle_parent_builtin_if_needed(cmds, env, pid))
			return;
		else
			launch_child_process(cmds, &in_fd, pipe_fd, pid, &i, env, first_cmds);
		cmds = cmds->next;
	}
	cmds = first_cmds;
	wait_for_pids(cmds, pid);
	close_fd(&in_fd, cmds, pipe_fd);
	free(pid);
}
