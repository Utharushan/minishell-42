#include "../includes/minishell.h"

int main(void)
{
    char *test_input = "echo \"42\"";
    t_token *tokens;
    t_command *cmds;
    t_env *env;
    
    /* Initialize environment */
    env = init_env(NULL, NULL);
    
    printf("Testing input: '%s'\n", test_input);
    
    /* Test lexer */
    tokens = lexer(test_input);
    printf("=== LEXER OUTPUT ===\n");
    print_tokens(tokens);
    
    /* Test parser */
    cmds = parse_tokens(tokens, env);
    printf("=== PARSER OUTPUT ===\n");
    print_commands(cmds);
    
    /* Cleanup */
    free_token_list(tokens);
    free_command_list(cmds);
    free_env_list(env);
    
    return 0;
} 