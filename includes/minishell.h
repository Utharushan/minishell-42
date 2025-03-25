# ifndef MINISHELL_H
# define MINISHELL_H


#include <stddef.h>

typedef struct s_env
{
    char            *name;
    char            *value;
    struct s_env    *next;
}               t_env;

# endif