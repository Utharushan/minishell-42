#include "minishell.h"

t_command	*new_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		return (NULL);
	}
	cmd->args = NULL;
	cmd->next = NULL;
	cmd->input = NULL;
	cmd->output = NULL;
	cmd->append = 0;
	return (cmd);
}

void	add_argument(t_command *cmd, char *arg)
{
	int	count;
	char	**new_args;
	int	i;

	count = 0;
	while (cmd->args && cmd->args[count])
	{
		count++;
	}
	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
	{
		return;
	}
	i = 0;
	while (i < count)
	{
		new_args[i] = cmd->args[i];
		i++;
	}
	new_args[count] = arg;
	new_args[count + 1] = NULL;
	free(cmd->args);
	cmd->args = new_args;
}

static void	handle_redirection(t_command *cmd, t_token **tokens)
{
	t_token	*next_token;

	next_token = (*tokens)->next;
	if (!next_token || next_token->type != TOKEN_WORD)
	{
		return;
	}
	if ((*tokens)->type == TOKEN_REDIRECT_IN)
	{
		cmd->input = next_token->value;
	}
	else
	{
		cmd->output = next_token->value;
		cmd->append = ((*tokens)->type == TOKEN_REDIRECT_APPEND);
	}
	*tokens = next_token;
}

static t_command	*handle_pipe(t_command *cmd)
{
	cmd->next = new_command();
	return (cmd->next);
}

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
		else if (tokens->type == TOKEN_REDIRECT_IN || tokens->type == TOKEN_REDIRECT_OUT
			|| tokens->type == TOKEN_REDIRECT_APPEND)
		{
			handle_redirection(cmd, &tokens);
		}
		tokens = tokens->next;
	}
	return (head);
}
