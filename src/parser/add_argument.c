#include "../../includes/minishell.h"

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