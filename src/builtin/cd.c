# include "../types/envm.h"
# include "../../lib/libft/libft.h"
# include <unistd.h>
# include <stdio.h>

int	builtin_cd(char **args, t_env **env_list)
{
	char	*path;
	char	buf[4096];
	char	*old_pwd;

	if (!args[1])
	{
		path = envm_get(*env_list, "HOME");
		if (!path)
		{
			write(2, "minishell: cd: HOME not set\n", 28);
			return (1);
		}
	}
	else
		path = args[1];
	old_pwd = envm_get(*env_list, "PWD");
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (old_pwd)
		envm_set(env_list, "OLDPWD", old_pwd);
	if (getcwd(buf, sizeof(buf)))
		envm_set(env_list, "PWD", buf);
	return (0);
}
