/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 21:04:42 by ebella            #+#    #+#             */
/*   Updated: 2025/07/04 21:12:33 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	sigint_handler(int sig)
{
	char	c;

	(void)sig;
	g_signal_status = 130;
	c = '\0';
	write(1, "\n", 1);
	write(1, "\033[2K", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	ioctl(STDIN_FILENO, TIOCSTI, &c);
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
		return (NULL);
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

int	check_input(char *input)
{
	int	i;

	i = 0;
	while (input[i] == ' ' || input[i] == '\t')
		i++;
	if (input[i] == '\0')
		return (1);
	return (0);
}
