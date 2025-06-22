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
        printf("DEBUG LEXER MAIN: At index %d, char: '%c' (ascii: %d)\n", i, input[i], input[i]);
        if (ft_isspace(input[i]))
        {
            printf("DEBUG LEXER MAIN: Skipping whitespace at index %d\n", i);
            i++;
            continue ;
        }
        printf("DEBUG LEXER MAIN: Calling handle_token at index %d\n", i);
        handle_token(input, &i, &tokens);
        printf("DEBUG LEXER MAIN: After handle_token, index=%d\n", i);
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
	int		start;
	char	*word;
	int		j;

	start = *i;
	printf("DEBUG LEXER: Starting extract_word at index %d, char: '%c'\n", *i, input[*i]);
	
	// Handle quotes
	if (input[*i] == '\'' || input[*i] == '"')
	{
		char quote = input[*i];
		(*i)++;
		printf("DEBUG LEXER: Found quote '%c', moving to index %d\n", quote, *i);
		
		while (input[*i] && input[*i] != quote)
		{
			if (input[*i] == '\\' && quote == '"')
				(*i)++;
			(*i)++;
		}
		printf("DEBUG LEXER: After quote handling, index=%d\n", *i);
	}
	else
	{
		// Handle regular word
		while (input[*i] && !ft_isspace(input[*i]) 
			&& input[*i] != '\'' && input[*i] != '"'
			&& input[*i] != '|' && input[*i] != '<' 
			&& input[*i] != '>' && input[*i] != '&'
			&& input[*i] != '(' && input[*i] != ')'
			&& input[*i] != ';' && input[*i] != '\\')
		{
			(*i)++;
		}
		printf("DEBUG LEXER: After regular word, index=%d\n", *i);
	}
	
	printf("DEBUG LEXER: Word range: start=%d, end=%d\n", start, *i);
	
	// Extract the word
	word = malloc(sizeof(char) * (*i - start + 1));
	if (!word)
		return;
	
	j = 0;
	while (start < *i)
	{
		word[j] = input[start];
		printf("DEBUG LEXER: Copying char '%c' at position %d\n", input[start], j);
		start++;
		j++;
	}
	word[j] = '\0';
	
	printf("DEBUG LEXER: Extracted word: '%s' (length: %d)\n", word, j);
	add_token(tokens, word, TOKEN_WORD, WORD_UNQUOTED);
}

