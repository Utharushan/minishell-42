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
void	run_builtins(t_command *cmds, t_env *env)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return ;

	if (!ft_strncmp(cmds->args[0], "cd", 3))
		g_signal_status = ft_cd(cmds, env);
	else if (!ft_strncmp(cmds->args[0], "echo", 5))
	{
		if (cmds->args[1] && !ft_strncmp(cmds->args[1], "$?", 3))
		{
			ft_putnbr_fd(g_signal_status, 1);
			ft_putchar_fd('\n', 1);
		}
		else
			g_signal_status = ft_echo(cmds);
	}
	else if (!ft_strncmp(cmds->args[0], "pwd", 4))
		g_signal_status = ft_pwd();
	else if (!ft_strncmp(cmds->args[0], "env", 4))
		g_signal_status = ft_env(env);
	else if (!ft_strncmp(cmds->args[0], "export", 7))
		g_signal_status = ft_export(env, cmds->args);
	else if (!ft_strncmp(cmds->args[0], "unset", 6))
		g_signal_status = ft_unset(&env, cmds->args[1]);
}
int	is_builtins(t_command *cmds)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (1);

	if (!ft_strncmp(cmds->args[0], "cd", 3))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "echo", 5))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "pwd", 4))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "env", 4))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "export", 7))
		return (0);
	else if (!ft_strncmp(cmds->args[0], "unset", 6))
		return (0);
	else
		return (1);
}

static void	free_path_dirs(char **path_dirs)
{
	int i;

	if (!path_dirs)
		return ;
	i = 0;
	while (path_dirs[i])
		free(path_dirs[i++]);
	free(path_dirs);
}

static int	check_absolute_path(char *cmd_path)
{
	if (access(cmd_path, F_OK | X_OK) == 0)
		return (0);
	return (1);
}

int	find_cmd_in_path(t_command *cmd, t_env *env)
{
	char *full_path;
	char **path_dirs;
	int i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strchr(cmd->args[0], '/'))
		return (check_absolute_path(cmd->args[0]));
	path_dirs = get_path_dirs(env);
	if (!path_dirs)
		return (1);
	i = 0;
	while (path_dirs[i])
	{
		full_path = build_full_path(path_dirs[i], cmd->args[0]);
		if (!full_path)
		{
			free_path_dirs(path_dirs);
			return (1);
		}
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free(full_path);
			free_path_dirs(path_dirs);
			return (0);
		}
		free(full_path);
		i++;
	}
	free_path_dirs(path_dirs);
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

t_command	*init(t_token *tokens, t_command *cmds, char *input, t_env *env)
{
	tokens = lexer(input);
	if (!tokens)
	{
		g_signal_status = 2;
		free(input);
		return (NULL);
	}
	cmds = parse_tokens(tokens, env);
	if (count_cmds(cmds) > 512)
	{
		ft_putstr_fd("ERROR: too many commands\n", 2);
		return(NULL);
	}
	if (!cmds)
	{
		g_signal_status = 2;
		free_token_list(tokens);
		free(input);
		return (NULL);
	}
	free_token_list(tokens);
	free(input);
	return (cmds);
}

void	sigint_handler(int sig)
{
	(void)sig;
	char c;

	g_signal_status = 130;
	c = '\0';
	write(1, "\n", 1);
	write(1, "\033[2K", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	ioctl(STDIN_FILENO, TIOCSTI, &c);
}

int	main(int argc, char **argv, char **envp)
{
	t_token *tokens;
	t_command *cmds;
	char *input;
	t_env *env;

	(void)argc;
	(void)argv;
	cmds = NULL;
	tokens = NULL;
	env = init_env(envp, NULL);
	if (!env)
		return (1);
	while (1)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell> ");
		if (!input)
		{
			ft_putstr_fd("exit", 1);
			free_env_list(env);
			exit(0);
		}
		add_history(input);
		if (!check_input(input))
		{
			cmds = init(tokens, cmds, input, env);
			if (!cmds)
				continue ;
			if ((!cmds->next && cmds->args && cmds->args[0]
					&& find_cmd_in_path(cmds, env)) && is_builtins(cmds))
			{
				ft_putstr_fd(cmds->args[0], 2);
				ft_putstr_fd(": command not found\n", 2);
				g_signal_status = 127;
				free_command_list(cmds);
				continue ;
			}
			if (prepare_heredocs(cmds, env) == 0)
			{
				close_all_heredoc_fds(cmds);
				free_command_list(cmds);
				continue ;
			}
			run_pipe(cmds, env);
			close_all_heredoc_fds(cmds);
			free_command_list(cmds);
			if (g_signal_status == 130)
				continue ;
		}
		else
			free(input);
	}
	free_env_list(env);
	return (g_signal_status);
}
// valgrind --leak-check=full --trace-children=yes --track-fds=yes --suppressions=ignore_leak_readline.supp ./minishell
