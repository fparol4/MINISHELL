#include "../../../headers/minishell.h"

int bin_cd(char **args, t_env **env)
{
	char	*path;
	char	*path_old;
	char	buffer[BUFFER_SIZE];

	if (!args[1])
	{
		path = env_get(env, PWD_HOME);
		if (!path)
		{
			cm_message("cd: $HOME not set", 1);
			return (1);
		}
	}
	else
		path = args[1];
	path_old = env_get(env, ENV_PWD);
	if (chdir(path) != 0)
	{
		cm_perror("cd");
		return (1);
	}
	if (path_old)
		env_set(env, ENV_PWDO, path_old);
	if (getcwd(buffer, BUFFER_SIZE))
		env_set(env, ENV_PWDO, buffer);
	return (0);
}
