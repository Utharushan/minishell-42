/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 22:38:45 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/15 22:38:45 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	check_unclosed_quotes(const char *line)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (line[i])
	{
		if (line[i] == '\'' && dq % 2 == 0)
			sq++;
		else if (line[i] == '\"' && sq % 2 == 0)
			dq++;
		i++;
	}
	if (sq % 2 != 0 || dq % 2 != 0)
		return (1);
	return (0);
}

void	print_unclosed_quotes_error(const char *input)
{
	ft_putstr_fd("minishell: unexpected EOF", 2);
	if (input && *input == '\'')
		ft_putchar_fd('\'', 2);
	else if (input && *input == '"')
		ft_putchar_fd('"', 2);
	ft_putstr_fd("'\n", 2);
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
