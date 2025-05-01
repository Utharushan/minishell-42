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

void	print_tokens(t_token *tokens)
{
	while (tokens)
	{
		ft_printf("TOKEN: [%s] Type: %d\n", tokens->value, tokens->type);
		tokens = tokens->next;
	}
}

void	print_commands(t_command *cmds)
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
int	is_builtins(t_command *cmds)
{
	if (!ft_strncmp(cmds->args[0], "echo", 4))
		ft_echo(cmds);
	else if (!ft_strncmp(cmds->args[0], "pwd", 3))
		ft_pwd();
	else
		return (0);
	return (1);
}
int	check_input(char *input)
{
	int i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '\0')
		return (1);
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	t_token *tokens;
	t_command *cmds;
	char *input;
	pid_t pid;
	int status;

	(void)argv;
	(void)argc;
	(void)envp;
	while (1)
	{
		input = readline("minishell>");
		if (!check_input(input))
		{
			tokens = lexer(input);
			cmds = parse_tokens(tokens);
			command_exist(cmds, envp);
			// print_commands(cmds);
			if (is_builtins(cmds))
				;
			else
				pid = fork();
			if (pid == 0)
			{
				if (ft_strncmp(cmds->args[0], "exit", 4))
					exec_command(cmds, envp);
			}
			cmds->status = waitpid(pid, &status, WUNTRACED);
			if (!ft_strncmp(cmds->args[0], "exit", 4))
				ft_exit(cmds->status, cmds);
		}
	}
	return (0);
}
