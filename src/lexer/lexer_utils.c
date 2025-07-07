/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:39:54 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/31 13:39:54 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_tokens(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

void	reset_token_counts(t_token **old_tokens, int *token_count_before,
								int *token_count_after, t_token *tokens)
{
	*old_tokens = tokens;
	*token_count_before = count_tokens(tokens);
	*token_count_after = 0;
}

void	handle_token_cleanup(t_token **tokens, t_token *old_tokens,
								int token_count_before, int token_count_after)
{
	if (token_count_after == token_count_before && old_tokens != NULL)
	{
		free_token_list(old_tokens);
		*tokens = NULL;
	}
}

/*
Creates a new token with the given value and type.
Initializes the next pointer to NULL.
Returns a pointer to the new token, or NULL if allocation fails.
*/
t_token	*new_token(t_token_args args)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		if (args.value)
			free(args.value);
		return (NULL);
	}
	token->value = args.value;
	token->type = args.type;
	token->word_type = args.word_type;
	token->has_leading_space = args.has_leading_space;
	token->next = NULL;
	return (token);
}

/*
Adds a new token with the given value and type to the end of the token list.
If the list is empty, sets the new token as the head.
Does nothing if token allocation fails.
*/
void	add_token(t_token **tokens, t_token_args args)
{
	t_token	*new;
	t_token	*temp;

	new = new_token(args);
	if (!new)
	{
		if (args.value)
			free(args.value);
		return ;
	}
	if (!*tokens)
		*tokens = new;
	else
	{
		temp = *tokens;
		while (temp->next)
			temp = temp->next;
		temp->next = new;
	}
}
