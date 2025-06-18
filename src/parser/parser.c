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

static void	add_redir(t_command *cmd, int type, char *file, int heredoc_expand)
{
    t_redir *new;
	t_redir *tmp;

	new = malloc(sizeof(t_redir));
    if (!new)
        return;
    new->file = ft_strdup(file);
    new->type = type;
    new->heredoc_expand = heredoc_expand;
    new->next = NULL;
    if (!cmd->redir)
        cmd->redir = new;
    else
    {
        tmp = cmd->redir;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = new;
    }
}

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
	cmd->redir = NULL;
	return (cmd);
}

/*
Adds an argument to the command's argument list.
Allocates a new array with space for the new argument and NULL terminator.
Copies existing arguments and appends the new one.
Frees the old argument array.
*/
void	add_argument(t_command *cmd, char *arg, t_env *env)
{
    int		count;
    char	**new_args;
    int		i;
    char	*final_arg = NULL;

    count = 0;
    while (cmd->args && cmd->args[count])
        count++;

    // Always use expand_token_value, which now handles all quoting and expansion
    final_arg = expand_token_value(arg, env, g_signal_status);

    new_args = malloc(sizeof(char *) * (count + 2));
    if (!new_args)
    {
        free(final_arg);
        return ;
    }
    i = 0;
    while (i < count)
    {
        new_args[i] = cmd->args[i];
        i++;
    }
    new_args[count] = final_arg;
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
t_command	*parse_tokens(t_token *tokens, t_env *env)
{
    t_command	*cmd;
    t_command	*head;
    t_token_type type;

    cmd = new_command();
    head = cmd;
    while (tokens && tokens->type != TOKEN_EOF)
    {
        if (tokens->type == TOKEN_WORD)
        {
            add_argument(cmd, tokens->value, env);
        }
        else if (tokens->type == TOKEN_PIPE)
        {
            cmd = handle_pipe(cmd);
        }
        else if (tokens->type == TOKEN_REDIRECT_IN
            || tokens->type == TOKEN_REDIRECT_OUT
            || tokens->type == TOKEN_REDIRECT_APPEND)
        {
            type = tokens->type;
            tokens = tokens->next;
            if (!tokens || tokens->type != TOKEN_WORD)
            {
                ft_printf("minishell: parse error near `\\n'\n");
                return head;
            }
            add_redir(cmd, type, tokens->value, 0);
        }
        else if (tokens->type == TOKEN_HEREDOC)
        {
            tokens = tokens->next;
            if (!tokens || tokens->type != TOKEN_WORD)
            {
                ft_printf("minishell: parse error near `\\n'\n");
                return head;
            }
            int expand = (tokens->word_type == WORD_UNQUOTED);
            add_redir(cmd, TOKEN_HEREDOC, tokens->value, expand);
        }
        tokens = tokens->next;
    }
    return (head);
}
