/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_tokens_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/11 22:00:00 by tuthayak          #+#    #+#             */
/*   Updated: 2025/07/11 22:00:00 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	is_directory(char *path)
{
	struct stat	st;

	if (stat(path, &st) == 0 && S_ISDIR(st.st_mode))
		return (1);
	return (0);
}

static char	*heredoc_delim_token(t_token *token)
{
	int		len;
	char	*delim;

	delim = token->value;
	if (token->word_type == WORD_SINGLE_QUOTED
		|| token->word_type == WORD_DOUBLE_QUOTED)
	{
		len = ft_strlen(delim);
		if (len >= 2 && ((delim[0] == '\'' && delim[len - 1] == '\'')
				|| (delim[0] == '"' && delim[len - 1] == '"')))
			return (ft_substr(delim, 1, len - 2));
		return (ft_strdup(delim));
	}
	return (ft_strdup(delim));
}

int	handle_redirection_tokens(t_command *cmd, t_token **tokens, t_command *head)
{
	t_token_type	type;

	type = (*tokens)->type;
	*tokens = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
	{
		ft_printf("minishell: parse error near `\\n'\n");
		free_command_list(head);
		return (0);
	}
	if (!add_redir(cmd, type, (*tokens)->value, 0))
	{
		free_command_list(head);
		return (0);
	}
	return (1);
}

static int	add_heredoc_redir(t_command *cmd, t_token *token, int expand,
	t_command *head)
{
	char	*tmp;
	int		ok;

	tmp = heredoc_delim_token(token);
	if (!tmp)
		return (0);
	ok = add_redir(cmd, TOKEN_HEREDOC, tmp, expand);
	free(tmp);
	if (!ok)
	{
		free_command_list(head);
		return (0);
	}
	return (1);
}

int	handle_heredoc_tokens(t_command *cmd, t_token **tokens, t_command *head)
{
	int	expand;

	*tokens = (*tokens)->next;
	if (!(*tokens) || (*tokens)->type != TOKEN_WORD)
	{
		ft_printf("minishell: parse error near `\\n'\n");
		free_command_list(head);
		return (0);
	}
	expand = ((*tokens)->word_type == WORD_UNQUOTED);
	return (add_heredoc_redir(cmd, *tokens, expand, head));
}
