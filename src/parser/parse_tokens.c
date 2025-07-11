#include "../../includes/minishell.h"

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