/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebella <ebella@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:09:21 by ebella            #+#    #+#             */
/*   Updated: 2025/03/25 15:23:52 by ebella           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static  char    *env_path(t_env *env, const char *var, size_t len)
{
    char    *oldpwd;
    int     i;
    int     j;
    int     s_alloc;

    while (env && env->next)
    {
        if (ft_strncmp(env->name, var, len) == 0)
        {
            s_alloc = ft_strlen(env->value) + 1;
            oldpwd = (char *)malloc(sizeof(char) * s_alloc);
            if (!oldpwd)
                return (NULL);
            i = 0;
            j = 0;
            while (env->value[i])
            {
                oldpwd[j] = env->value[i];
                i++;
                j++;
            }
            oldpwd[j] = '\0';
            return (oldpwd);
        }
        env = env->next;
    }
    return (NULL);
}
