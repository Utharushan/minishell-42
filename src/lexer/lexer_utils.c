/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 13:39:54 by tuthayak          #+#    #+#             */
/*   Updated: 2025/03/31 13:39:54 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

/*
Creates a new token with the given value and type.
Initializes the next pointer to NULL.
Returns a pointer to the new token, or NULL if allocation fails.
*/
t_token *new_token(char *value, t_token_type type)
{
	t_token *token;

	token = malloc(sizeof(t_token));
	if (!token)
	{
		return (NULL);
	}
	token->value = value;
	token->type = type;
	token->next = (NULL);
	return (token);
}

/*
Adds a new token with the given value and type to the end of the token list.
If the list is empty, sets the new token as the head.
Does nothing if token allocation fails.
*/
void add_token(t_token **tokens, char *value, t_token_type type)
{
	t_token *new;
	t_token *temp;

	new = new_token(value, type);
	if (!new)
	{
		return;
	}
	if (!*tokens)
	{
		*tokens = new;
	}
	else
	{
		temp = *tokens;
		while (temp->next)
		{
			temp = temp->next;
		}
		temp->next = new;
	}
}
