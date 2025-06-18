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
Continues until a special character or whitespace is found.
Adds the extracted word as a token and updates the index.
*/
void	extract_word(char *input, int *i, t_token **tokens)
{
    int			start;
    char		quote;
    char		*result;
    char		*segment;
    t_word_type	wt = WORD_UNQUOTED;
    int			quoted = 0;

    result = ft_strdup("");
    while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|' && input[*i] != '<' && input[*i] != '>')
    {
        if ((input[*i] == '"' || input[*i] == '\'') && !quoted)
        {
            quote = input[*i];
            quoted = 1;
            wt = (quote == '\'') ? WORD_SINGLE_QUOTED : WORD_DOUBLE_QUOTED;
            start = *i;
            (*i)++;
            while (input[*i] && input[*i] != quote)
                (*i)++;
            if (input[*i] == quote)
                (*i)++;
            // Include the quotes in the token value
            segment = ft_substr(input, start, *i - start);
            result = ft_strjoin(result, segment);
            free(segment);
        }
        else
        {
            start = *i;
            while (input[*i] && !ft_isspace(input[*i]) && input[*i] != '|'
                    && input[*i] != '<' && input[*i] != '>' && input[*i] != '"' && input[*i] != '\'')
                (*i)++;
            segment = ft_substr(input, start, *i - start);
            result = ft_strjoin(result, segment);
            free(segment);
        }
    }
    add_token(tokens, result, TOKEN_WORD, wt);
    (*i)--;
}

