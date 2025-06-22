/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:39:52 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/31 13:39:52 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Handles the extraction and addition of a token from the input string at position *i.
Determines the token type and calls extract_word or adds a token accordingly.
*/
void	handle_token(char *input, int *i, t_token **tokens)
{
    t_token_type	type;
    int				length;

    type = get_token_type(input, i);
    if (type == TOKEN_WORD)
    {
        extract_word(input, i, tokens);
    }
    else
    {
        if (type == TOKEN_HEREDOC || type == TOKEN_REDIRECT_APPEND)
            length = 2;
        else
            length = 1;
        add_token(tokens, ft_substr(input, *i, length), type, WORD_UNQUOTED);
    }
}

/*
Tokenizes the input string into a linked list of tokens.
Skips whitespace and processes each token using handle_token.
Appends a TOKEN_EOF at the end.
Returns the head of the token list.
*/
t_token	*lexer(char *input)
{
    t_token	*tokens;
    int		i;

    tokens = NULL;
    i = 0;
    while (input[i])
    {
        if (ft_isspace(input[i]))
        {
            i++;
            continue ;
        }
        handle_token(input, &i, &tokens);
        i++;
    }
    add_token(&tokens, NULL, TOKEN_EOF, WORD_UNQUOTED);
    return (tokens);
}

/*
Determines the type of token at the current position in the input string.
Handles special cases for heredoc and append redirections.
Advances the index for multi-character tokens.
Returns the detected token type.
*/
t_token_type	get_token_type(char *input, int *i)
{
	if (input[*i] == '|')
	{
		return (TOKEN_PIPE);
	}
	else if (input[*i] == '<')
	{
		if (input[*i + 1] == '<')
		{
			(*i)++;
			return (TOKEN_HEREDOC);
		}
		return (TOKEN_REDIRECT_IN);
	}
	else if (input[*i] == '>')
	{
		if (input[*i + 1] == '>')
		{
			(*i)++;
			return (TOKEN_REDIRECT_APPEND);
		}
		return (TOKEN_REDIRECT_OUT);
	}
	return (TOKEN_WORD);
}

/*
Extracts a word token from the input string starting at position *i.
Extracts the complete word including quotes and variables as a single token.
Adds the extracted word as a token and updates the index.
*/
void	extract_word(char *input, int *i, t_token **tokens)
{
	int start;
	t_word_type word_type = WORD_UNQUOTED;
	char quote;

	start = *i;
	while (input[*i] && !ft_isspace(input[*i]) && !is_token_delim(input[*i]))
	{
		if (input[*i] == '\'' || input[*i] == '"')
		{
			quote = input[*i];
			if (quote == '\'')
				word_type = WORD_SINGLE_QUOTED;
			else if (word_type != WORD_SINGLE_QUOTED)
				word_type = WORD_DOUBLE_QUOTED;
			(*i)++;
			while (input[*i] && input[*i] != quote)
				(*i)++;
			if (input[*i] == quote)
				(*i)++;
		}
		else if (input[*i] == '$')
		{
			(*i)++;
			if (input[*i] == '?')
				(*i)++;
			else
				while (input[*i] && (ft_isalnum(input[*i]) || input[*i] == '_'))
					(*i)++;
		}
		else
		{
			(*i)++;
		}
	}
	if (*i > start)
		add_token(tokens, ft_substr(input, start, *i - start), TOKEN_WORD, word_type);
	(*i)--;
}

