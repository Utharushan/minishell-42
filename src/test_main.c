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


int			g_signal_status = 0;

int singleton(int type, int value)
{
	static int	nb = 0;

	if (type == 0)
		return (nb);
	else
		nb = value;
	return (nb);
}

int	handle_input_error(char *input, t_env *env)
{
	if (!input)
	{
		ft_putstr_fd("exit", 1);
		if (env)
			free_env_list(env);
		exit(singleton(0, 0));
	}
	return (0);
}

int	handle_command_not_found(t_command *cmds, t_env *env)
{
	if ((!cmds->next && cmds->args && cmds->args[0]
			&& find_cmd_in_path(cmds, env)) && is_builtins(cmds))
	{
		ft_putstr_fd(cmds->args[0], 2);
		ft_putstr_fd(": command not found\n", 2);
		singleton(1, 127);
		free_command_list(cmds);
		return (1);
	}
	return (0);
}

int	execute_commands(t_command *cmds, t_env *env)
{
	if (prepare_heredocs(cmds, env) == 0)
	{
		close_all_heredoc_fds(cmds);
		free_command_list(cmds);
		return (1);
	}
	run_pipe(cmds, env);
	if (g_signal_status == 130)
		return (1);
	return (0);
}

void	process_input(char *input, t_token *tokens, t_command *cmds, t_env **env)
{
	if (!check_input(input))
	{
		cmds = init(tokens, cmds, input, *env);
		if (!cmds)
			return ;
		if (handle_command_not_found(cmds, *env))
			return ;
		execute_commands(cmds, *env);
	}
	else
		free(input);
}

int	main(int argc, char **argv, char **envp)
{
	t_token		*tokens;
	t_command	*cmds;
	char		*input;
	t_env		*env;

	(void)argc;
	(void)argv;
	cmds = NULL;
	tokens = NULL;
	env = init_env(envp, NULL);
	if (!env)
		return (1);
	while (1)
	{
		setup_signals();
		input = readline("minishell> ");
		handle_input_error(input, env);
		add_history(input);
		process_input(input, tokens, cmds, &env);
	}
	free_env_list(env);
	return (singleton(0, 0));
}
// valgrind --leak-check=full --trace-children=yes --track-fds=yes --suppressions=ignore_leak_readline.supp ./minishell