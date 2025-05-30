/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:39:56 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/31 13:39:56 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Allocates and initializes a new command structure.
Sets all fields to default values (NULL, false, OP_NONE).
Returns a pointer to the new command, or NULL if allocation fails.
*/
t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->append = false;
	cmd->next_op = OP_NONE;
	cmd->next = NULL;
	cmd->heredoc_delim = NULL;
	return (cmd);
}

/*
Adds an argument to the command's argument list.
Allocates a new array with space for the new argument and NULL terminator.
Copies existing arguments and appends the new one.
Frees the old argument array.
*/
void	add_argument(t_command *cmd, char *arg)
{
    int		count;
    char	**new_args;
    int		i;
    char	*stripped_arg;

    count = 0;
    while (cmd->args && cmd->args[count])
    {
        count++;
    }
    if ((arg[0] == '"' && arg[ft_strlen(arg) - 1] == '"') ||
        (arg[0] == '\'' && arg[ft_strlen(arg) - 1] == '\''))
        stripped_arg = ft_substr(arg, 1, ft_strlen(arg) - 2);
    else
        stripped_arg = arg;
    new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
    {
        if (stripped_arg != arg)
            free(stripped_arg);
        return ;
    }
    i = 0;
    while (i < count)
    {
        new_args[i] = cmd->args[i];
        i++;
    }
    new_args[count] = stripped_arg;
    new_args[count + 1] = NULL;
    free(cmd->args);
    cmd->args = new_args;
}

/*
Handles a pipe operator in the command list.
Sets the current command's next_op to OP_PIPE and creates a new command for the next segment.
Returns a pointer to the new command.
*/
t_command	*handle_pipe(t_command *cmd)
{
	cmd->next_op = OP_PIPE;
	cmd->next = new_command();
	return (cmd->next);
}

/*
Parses a list of tokens into a linked list of command structures.
Handles arguments, pipes, redirections, and heredocs.
Returns the head of the command list.
*/
t_command	*parse_tokens(t_token *tokens)
{
	t_command	*cmd;
	t_command	*head;

	cmd = new_command();
	head = cmd;
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_WORD)
		{
			add_argument(cmd, tokens->value);
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			cmd = handle_pipe(cmd);
		}
		else if (tokens->type == TOKEN_REDIRECT_IN
			|| tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_REDIRECT_APPEND)
		{
			handle_redirection(cmd, &tokens);
		}
		else if (tokens->type == TOKEN_HEREDOC)
		{
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				cmd->here_doc = TOKEN_HEREDOC;
				cmd->heredoc_delim = tokens->value;
			}
		}
		tokens = tokens->next;
	}
	return (head);
}
