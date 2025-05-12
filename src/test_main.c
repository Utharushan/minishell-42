/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+
	+:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+
	+#+        */
/*                                                +#+#+#+#+#+
	+#+           */
/*   Created: 2025/04/06 15:37:01 by tuthayak          #+#    #+#             */
/*   Updated: 2025/04/06 15:37:01 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int g_signal_status = 0;

void print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_printf("TOKEN: [%s] Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

void print_commands(t_command *cmds)
{
	int i;

	while (cmds)
	{
		ft_printf("COMMAND:\n");
		i = 0;
		while (cmds->args && cmds->args[i])
		{
			ft_printf("  args[%d]: %s\n", i, cmds->args[i]);
			i++;
		}
		ft_printf("  next_op: ");
		if (cmds->next_op == OP_PIPE)
			ft_printf("|\n");
		else if (cmds->next_op == OP_AND)
			ft_printf("&&\n");
		else if (cmds->next_op == OP_OR)
			ft_printf("||\n");
		else
			ft_printf("NONE\n");
		cmds = cmds->next;
	}
}

int is_builtins(t_command *cmds)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);

	if (!ft_strncmp(cmds->args[0], "echo", 5))
	{
		if (cmds->args[1] && !ft_strncmp(cmds->args[1], "$?", 3))
		{
			ft_putnbr_fd(g_signal_status, 1);
			ft_putchar_fd('\n', 1);
			g_signal_status = 0;
		}
		else
			g_signal_status = ft_echo(cmds);
	}
	else if (!ft_strncmp(cmds->args[0], "pwd", 4))
		g_signal_status = ft_pwd();
	else if (!ft_strncmp(cmds->args[0], "exit", 5))
	{
		ft_exit(cmds->args);
		cmds->status = g_signal_status;
	}
	else
		return (0);
	return (1);
}

int find_cmd_in_path(t_command *cmds)
{
	char *full_path;
	int i;

	if (!cmds || !cmds->path || !cmds->args[0])
		return (1);

	i = 0;
	while (cmds->path[i])
	{
		full_path = ft_strjoin(cmds->path[i], "/");
		if (!full_path)
			return (1);
		char *tmp = ft_strjoin(full_path, cmds->args[0]);
		free(full_path);
		if (!tmp)
			return (1);
		if (access(tmp, F_OK | X_OK) == 0)
		{
			free(tmp);
			return (0);
		}
		free(tmp);
		i++;
	}
	return (1);
}

int check_input(char *input)
{
	int i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '\0')
		return (1);
	return (0);
}

int main(int argc, char **argv, char **envp)
{
	t_token *tokens = NULL;
	t_command *cmds = NULL;
	char *input;
	pid_t pid;
	int status;

	(void)argc;
	(void)argv;

	while (1)
	{
		input = readline("minishell>");
		if (!check_input(input))
		{
			tokens = lexer(input);
			cmds = parse_tokens(tokens);
			init_command_path(cmds, envp);
			free(input);

			if (is_builtins(cmds))
				g_signal_status = cmds->status;
			else if (find_cmd_in_path(cmds))
			{
				ft_putstr_fd(cmds->args[0], 2);
				ft_putstr_fd(": command not found\n", 2);
				g_signal_status = 127;
			}
			else
			{
				pid = fork();
				if (pid == 0)
				{
					exec_command(cmds, envp);
					ft_exit(cmds->args);
				}
				waitpid(pid, &status, 0);
				if (WIFEXITED(status))
					g_signal_status = WEXITSTATUS(status);
				else
					g_signal_status = 1;
			}
			free_struct(cmds);
			cmds = NULL;
		}
		else
			free(input);
	}
	return (0);
}
