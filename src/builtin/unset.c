# include "../types/envm.h"

int	builtin_unset(char **args, t_env **env_list)
{
	int	i;

	i = 1;
	while (args[i])
	{
		envm_unset(env_list, args[i]);
		i++;
	}
	return (0);
}
