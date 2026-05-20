#include "../../../headers/minishell.h"

int bin_pwd(char **args, t_env **env)
{
	char buffer[4096];
	char *fallback;

	(void)args;
	if (getcwd(buffer, sizeof(buffer)))
	{
		ft_putendl_fd(buffer, 1);
		return (0);
	}
	fallback = env_get(env, ENV_PWD);
	if (fallback)
		ft_putendl_fd(fallback, 1);
	return (1);
}
