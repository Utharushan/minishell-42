/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+        
	+:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+      
	+#+        */
/*                                                +#+#+#+#+#+  
	+#+           */
/*   Created: 2025/03/31 13:39:56 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/31 13:39:56 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"


static int	add_redir(t_command *cmd, int type, char *file, int heredoc_expand)
{
	t_redir *new;
	t_redir *tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return (0);
	new->file = ft_strdup(file);
	if (!new->file)
	{
		free(new);
		return (0);
	}
	new->type = type;
	new->heredoc_expand = heredoc_expand;
	new->heredoc_fd = -1;
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
	return (1);
}

/*
Allocates and initializes a new command structure.
Sets all fields to default values (NULL, false, OP_NONE).
Returns a pointer to the new command, or NULL if allocation fails.
*/
t_command	*new_command(void)
{
	t_command *cmd;

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
int add_argument(t_command *cmd, char *arg, t_word_type word_type, t_env *env)
{
	int count;
	char **new_args;
	int i;
	char *final_arg = NULL;

	count = 0;
	while (cmd->args && cmd->args[count])
		count++;

	final_arg = expand_token_value(arg, word_type, env, singleton(0, 0));
	if (!final_arg)
	{
		return (0);
	}

	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
	{
		free(final_arg);
		return (0);
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
	return (1);
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

int add_argument_concat(t_command *cmd, t_token **tokens, t_env *env)
{
	char *arg = ft_strdup("");
	char *tmp;
	int first = 1;
	if (!arg)
		return (0);
	while (*tokens && (*tokens)->type == TOKEN_WORD)
	{
		if (!first && (*tokens)->has_leading_space)
			break ;
		char *expanded = expand_token_value((*tokens)->value,
				(*tokens)->word_type, env, singleton(0, 0));
		tmp = arg;
		arg = ft_strjoin(arg, expanded);
		free(tmp);
		free(expanded);
		*tokens = (*tokens)->next;
		first = 0;
	}
	int count = 0;
	while (cmd->args && cmd->args[count])
		count++;
	char **new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
	{
		free(arg);
		return (0);
	}
	int i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = ft_strdup(arg);
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
	free(arg);
	return (1);
}

/*
Parses a list of tokens into a linked list of command structures.
Handles arguments, pipes, redirections, and heredocs.
Returns the head of the command list.
*/
t_command	*parse_tokens(t_token *tokens, t_env *env)
{
	if (check_syntax_errors(tokens))
	{
		return (NULL);
	}
	t_command *cmd;
	t_command *head;
	t_token_type type;

	cmd = new_command();
	head = cmd;
	if (tokens && tokens->type == TOKEN_WORD)
	{
		if (!add_argument_concat(cmd, &tokens, env))
		{
			free_command_list(head);
			return (NULL);
		}
		if (cmd->args && cmd->args[0])
		{
			struct stat st;
			if (stat(cmd->args[0], &st) == 0 && S_ISDIR(st.st_mode))
			{
				ft_putstr_fd(cmd->args[0], 2);
				ft_putstr_fd(": Is a directory\n", 2);
				free_command_list(head);
				return (NULL);
			}
		}
	}
	while (tokens && tokens->type != TOKEN_EOF)
	{
		if (tokens->type == TOKEN_WORD)
		{
			if (!add_argument_concat(cmd, &tokens, env))
			{
				free_command_list(head);
				return (NULL);
			}
			continue ;
		}
		else if (tokens->type == TOKEN_PIPE || tokens->type == TOKEN_SEMICOLON)
		{
			cmd = handle_pipe(cmd);
			tokens = tokens->next;
			if (tokens && tokens->type == TOKEN_WORD)
			{
				if (!add_argument(cmd, tokens->value, tokens->word_type, env))
				{
					free_command_list(head);
					return (NULL);
				}
				tokens = tokens->next;
			}
			continue ;
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
				free_command_list(head);
				return (NULL);
			}
			if (!add_redir(cmd, type, tokens->value, 0))
			{
				free_command_list(head);
				return (NULL);
			}
		}
		else if (tokens->type == TOKEN_HEREDOC)
		{
			tokens = tokens->next;
			if (!tokens || tokens->type != TOKEN_WORD)
			{
				ft_printf("minishell: parse error near `\\n'\n");
				free_command_list(head);
				return (NULL);
			}
			int expand = (tokens->word_type == WORD_UNQUOTED);
			char *delim = tokens->value;
			if (tokens->word_type == WORD_SINGLE_QUOTED
				|| tokens->word_type == WORD_DOUBLE_QUOTED)
			{
				int len = ft_strlen(delim);
				if (len >= 2 && ((delim[0] == '\'' && delim[len - 1] == '\'')
						|| (delim[0] == '"' && delim[len - 1] == '"')))
					delim = ft_substr(delim, 1, len - 2);
				else
					delim = ft_strdup(delim);
			}
			else
				delim = ft_strdup(delim);
			if (!add_redir(cmd, TOKEN_HEREDOC, delim, expand))
			{
				free(delim);
				free_command_list(head);
				return (NULL);
			}
			free(delim);
		}
		tokens = tokens->next;
	}
	return (head);
}
