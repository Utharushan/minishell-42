/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 22:01:13 by ebella            #+#    #+#             */
/*   Updated: 2025/07/10 22:09:26 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*extract_env_name(char *env_str, char *equal_pos)
{
	char	*name;

	name = ft_substr(env_str, 0, equal_pos - env_str);
	return (name);
}

char	*extract_env_value(char *equal_pos)
{
	char	*value;

	value = ft_strdup(equal_pos + 1);
	return (value);
}

static char	*create_env_string(t_env *env_node)
{
	char	*tmp_str;
	char	*result;

	tmp_str = ft_strjoin(env_node->name, "=");
	if (!tmp_str)
		return (NULL);
	result = ft_strjoin(tmp_str, env_node->value);
	free(tmp_str);
	return (result);
}

char	**env_to_envp(t_env *env)
{
	int		count;
	int		i;
	t_env	*tmp;
	char	**envp;

	i = 0;
	tmp = env;
	count = count_env(tmp);
	envp = malloc(sizeof(char *) * (count + 1));
	if (!envp)
		return (NULL);
	tmp = env;
	while (tmp)
	{
		envp[i] = create_env_string(tmp);
		if (!envp[i])
			return (free_envp(envp, i), NULL);
		i++;
		tmp = tmp->next;
	}
	envp[i] = NULL;
	return (envp);
}
