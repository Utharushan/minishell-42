/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/06 15:37:01 by tuthayak          #+#    #+#             */
/*   Updated: 2025/04/06 15:37:01 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

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
	int	i;

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

int	main(int argc, char **argv)
{
	t_token		*tokens;
	t_command	*cmds;

	if (argc != 2)
	{
		ft_printf("Usage: %s \"command line\"\n", argv[0]);
		return (1);
	}
	tokens = lexer(argv[1]);
	if (!tokens)
	{
		ft_printf("Lexer failed.\n");
		return (1);
	}
	ft_printf("==== Tokens ====\n");
	print_tokens(tokens);
	cmds = parse_tokens(tokens);
	if (!cmds)
	{
		ft_printf("Parser failed.\n");
		return (1);
	}
	ft_printf("\n==== Commands ====\n");
	print_commands(cmds);
	return (0);
}
