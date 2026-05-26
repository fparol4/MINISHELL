#include "../../../headers/runner.h"

int	rn_exec_bin(char **args, t_env **env, int *status)
{
	if (!args || !args[0])
		return (0);
	if (ft_strcmp(args[0], "echo") == 0)
		return (*status = bin_echo(args + 1, env), 1);
	if (ft_strcmp(args[0], "cd") == 0)
		return (*status = bin_cd(args + 1, env), 1);
	if (ft_strcmp(args[0], "pwd") == 0)
		return (*status = bin_pwd(args + 1, env), 1);
	if (ft_strcmp(args[0], "export") == 0)
		return (*status = bin_export(args + 1, env), 1);
	if (ft_strcmp(args[0], "unset") == 0)
		return (*status = bin_unset(args + 1, env), 1);
	if (ft_strcmp(args[0], "env") == 0)
		return (*status = bin_env(args + 1, env), 1);
	if (ft_strcmp(args[0], "exit") == 0)
		return (*status = bin_exit(args + 1, env), 1);
	return (0);
}
