/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:46 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:46 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_external.h"

int	rn_ext_execfail(char *path)
{
	if (errno == ENOENT)
	{
		sh_err2(NULL, path, "No such file or directory");
		return (127);
	}
	if (errno == EISDIR)
	{
		sh_err2(NULL, path, "Is a directory");
		return (126);
	}
	if (errno == EACCES)
	{
		sh_err2(NULL, path, "Permission denied");
		return (126);
	}
	if (errno == ENOEXEC)
	{
		sh_err2(NULL, path, "Exec format error");
		return (126);
	}
	sh_err2(NULL, path, strerror(errno));
	return (126);
}

void	rn_ext_child(char *path, char **args, char **envp)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(path, args, envp);
	_exit(rn_ext_execfail(path));
}

int	rn_ext_wait(pid_t pid)
{
	int	status;
	int	ret;

	if (waitpid(pid, &status, 0) == -1)
	{
		sh_sig_mode(SIG_INTERACTIVE);
		return (1);
	}
	sh_sig_mode(SIG_INTERACTIVE);
	if (WIFEXITED(status))
	{
		ret = WEXITSTATUS(status);
		return (ret);
	}
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	ext_fork(char *path, char **args, char **envp)
{
	pid_t	pid;

	sh_sig_mode(SIG_EXEC);
	pid = fork();
	if (pid == -1)
	{
		sh_sig_mode(SIG_INTERACTIVE);
		return (-1);
	}
	if (pid == 0)
		rn_ext_child(path, args, envp);
	return (rn_ext_wait(pid));
}

int	rn_exec_ext(char **args, t_env **env)
{
	char	*path;
	char	**envp;
	int		status;

	if (!args || !args[0])
		return (0);
	status = rn_ext_resolve(args, env, &path);
	if (status)
	{
		free(path);
		return (status);
	}
	envp = env_toarr(env);
	if (!envp)
	{
		free(path);
		return (1);
	}
	status = ext_fork(path, args, envp);
	free(path);
	sh_freeargs(envp);
	if (status == -1)
		return (1);
	return (status);
}
