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
int g_exit_status = 0;

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
int run_builtins(t_command *cmds, t_env *env, t_minishell *mini)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);

	if (!ft_strncmp(cmds->args[0], "cd", 3))
		return (ft_cd(cmds, env));
	else if (!ft_strncmp(cmds->args[0], "echo", 5))
	{
		if (cmds->args[1] && !ft_strncmp(cmds->args[1], "$?", 3))
		{
			ft_putnbr_fd(mini->status, 1);
			ft_putchar_fd('\n', 1);
			return (0);
		}
		else
			return (ft_echo(cmds->args, env));
	}
	else if (!ft_strncmp(cmds->args[0], "pwd", 4))
		return (ft_pwd());
	else if (!ft_strncmp(cmds->args[0], "env", 4))
	{
		ft_env(env);
		return (0);
	}
	else if (!ft_strncmp(cmds->args[0], "export", 7))
	{
		ft_export(env, cmds->args);
		return (0);
	}
	else if (!ft_strncmp(cmds->args[0], "unset", 6))
	{
		ft_unset(&env, cmds->args[1]);
		return (0);
	}
	else
		return (127);
}
int is_builtins(t_command *cmds)
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

int find_cmd_in_path(t_command *cmd, t_env *env)
{
	char *full_path;
	char **path_dirs;
	int i;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strchr(cmd->args[0], '/'))
		return (access(cmd->args[0], F_OK | X_OK) == 0 ? 0 : 1);

	path_dirs = get_path_dirs(env);
	if (!path_dirs)
		return (1);

	i = 0;
	while (path_dirs[i])
	{
		full_path = build_full_path(path_dirs[i], cmd->args[0]);
		if (!full_path)
			break;
		if (access(full_path, F_OK | X_OK) == 0)
		{
			free(full_path);
			while (path_dirs[i])
				free(path_dirs[i++]);
			free(path_dirs);
			return (0);
		}
		free(full_path);
		i++;
	}
	i = 0;
	while (path_dirs[i])
		free(path_dirs[i++]);
	free(path_dirs);
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

t_command *init(t_token *tokens, t_command *cmds, char *input, t_env *env)
{
	tokens = lexer(input);
	cmds = parse_tokens(tokens, env);
	free_token_list(tokens);
	free(input);
	return (cmds);
}

void sigint_handler(int sig)
{
	(void)sig;
	g_signal_status = 130;	
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

t_minishell	*init_minishell(t_minishell *minishell)
{
	minishell = malloc(sizeof(t_minishell));
	if (!minishell)
		return (NULL);
	minishell->status = 0;
	return (minishell);
}

int main(int argc, char **argv, char **envp)
{
	t_token *tokens;
	t_command *cmds;
	t_minishell *mini;
	char *input;
	t_env *env;

	(void)argc;
	(void)argv;
	cmds = NULL;
	tokens = NULL;
	mini = NULL;
	env = init_env(envp, NULL);
	mini = init_minishell(mini);
	while (1)
	{
		signal(SIGINT, sigint_handler);
		signal(SIGQUIT, SIG_IGN);
		input = readline("minishell> ");
		add_history(input);
		if (!input)
		{
			ft_putstr_fd("exit", 1);
			mini->status = 130;
			exit(130);
		}
		if (!check_input(input))
		{
			cmds = init(tokens, cmds, input, env);
			if (!prepare_heredocs(cmds, env, mini))
			{
				free_command_list(cmds);
				continue;
			}
			run_pipe(cmds, env, mini);
			free_command_list(cmds);
			free_token_list(tokens);
			g_signal_status = 0;
		}
	}
	free_env_list(env);
	return (mini->status);
}
