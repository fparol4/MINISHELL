#include "../../../headers/runner.h"
#include <sys/wait.h>

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

int	rn_exec_ext(char **args, t_env **env)
{
	char	*path;
	char	**envp;
	pid_t	pid;
	int		status;

	if (!args || !args[0])
		return (0);
	path = rn_path(args, env);
	if (!path)
		return (sh_err2(NULL, args[0], "command not found"), 127);
	envp = env_toarr(env);
	if (!envp)
		return (free(path), 1);
	pid = fork();
	if (pid == -1)
		return (free(path), sh_freeargs(envp), 1);
	if (pid == 0)
	{
		execve(path, args, envp);
		sh_err2(NULL, args[0], "execution failed");
		exit(126);
	}
	free(path);
	sh_freeargs(envp);
	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	rn_exec_cmd(t_exnode *node, t_env **env)
{
	char	**args;
	int		status;

	if (!node || !node->args)
		return (0);
	args = rn_expand(node->args, env);
	if (!args)
		return (1);
	if (!args[0])
		status = 0;
	else if (!rn_exec_bin(args, env, &status))
		status = rn_exec_ext(args, env);
	sh_freeargs(args);
	return (status);
}

int	rn_exec_pipe(t_exnode *node, t_env **env)
{
	return (rn_pipe(node, env));
}

int	rn_execute(t_exnode *node, t_env **env)
{
	int	status;

	if (!node)
		return (0);
	if (node->type == CMD)
		status = rn_exec_cmd(node, env);
	else if (node->type == PIPE)
		status = rn_exec_pipe(node, env);
	else
		status = 1;
	rn_status_set(env, status);
	return (status);
}
