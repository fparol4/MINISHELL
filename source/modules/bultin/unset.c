#include "../../../headers/minishell.h"

int bin_unset(char **args, t_env **env)
{
	int i;

	i = 0;
	while (args[i])
		env_unset(env, args[i++]);
	return (0);
}
