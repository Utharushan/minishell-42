#include "../../includes/minishell.h"

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