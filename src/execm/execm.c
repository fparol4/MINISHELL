# include "../types/envm.h"
# include "../types/minishell.h"
# include "../builtin/builtins.h"
# include "../../lib/libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <stdio.h>

char	*execm_get_path(t_env *env_list, const char *cmd)
{
	char	*path_env;
	char	**dirs;
	char	*full;
	char	*tmp;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK | X_OK) == 0)
			return (ft_strdup(cmd));
		return (NULL);
	}
	path_env = envm_get(env_list, "PATH");
	if (!path_env)
		return (NULL);
	dirs = ft_split(path_env, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		tmp = ft_strjoin(dirs[i], "/");
		full = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full, F_OK | X_OK) == 0)
		{
			i = 0;
			while (dirs[i])
				free(dirs[i++]);
			free(dirs);
			return (full);
		}
		free(full);
		i++;
	}
	i = 0;
	while (dirs[i])
		free(dirs[i++]);
	free(dirs);
	return (NULL);
}

int	execm_is_builtin(const char *cmd)
{
	const char	*builtins[] = {"echo", "cd", "pwd", "export",
		"unset", "env", "exit", NULL};
	int			i;

	i = 0;
	while (builtins[i])
	{
		if (ft_strncmp(cmd, builtins[i], ft_strlen(builtins[i]) + 1) == 0)
			return (1);
		i++;
	}
	return (0);
}

int	execm_run_builtin(t_cmd *cmd_node, t_env **env_list)
{
	char	**a;

	a = cmd_node->args;
	if (ft_strncmp(a[0], "echo", 5) == 0)
		return (builtin_echo(a, env_list));
	if (ft_strncmp(a[0], "cd", 3) == 0)
		return (builtin_cd(a, env_list));
	if (ft_strncmp(a[0], "pwd", 4) == 0)
		return (builtin_pwd(a, env_list));
	if (ft_strncmp(a[0], "export", 7) == 0)
		return (builtin_export(a, env_list));
	if (ft_strncmp(a[0], "unset", 6) == 0)
		return (builtin_unset(a, env_list));
	if (ft_strncmp(a[0], "env", 4) == 0)
		return (builtin_env(a, env_list));
	if (ft_strncmp(a[0], "exit", 5) == 0)
		return (builtin_exit(a, env_list));
	return (1);
}

int	execm_exec_simple(t_cmd *cmd_node, t_env **env_list)
{
	char	*path;
	char	**env_arr;
	pid_t	pid;
	int		status;

	if (!cmd_node->args || !cmd_node->args[0])
		return (0);
	if (execm_is_builtin(cmd_node->args[0]))
		return (execm_run_builtin(cmd_node, env_list));
	path = execm_get_path(*env_list, cmd_node->args[0]);
	if (!path)
	{
		write(2, "minishell: ", 11);
		write(2, cmd_node->args[0], ft_strlen(cmd_node->args[0]));
		write(2, ": command not found\n", 20);
		return (127);
	}
	env_arr = envm_toarr(*env_list);
	pid = fork();
	if (pid == 0)
	{
		execve(path, cmd_node->args, env_arr);
		perror(cmd_node->args[0]);
		free(path);
		free(env_arr);
		exit(126);
	}
	free(path);
	free(env_arr);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}
