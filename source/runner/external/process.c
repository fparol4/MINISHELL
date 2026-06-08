/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 12:55:12 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 12:55:12 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_external.h"

int	rn_ext_execfail(char *path)
{
	if (errno == ENOENT)
		return (sh_err2(NULL, path, "No such file or directory"), 127);
	if (errno == EISDIR)
		return (sh_err2(NULL, path, "Is a directory"), 126);
	if (errno == EACCES)
		return (sh_err2(NULL, path, "Permission denied"), 126);
	if (errno == ENOEXEC)
		return (sh_err2(NULL, path, "Exec format error"), 126);
	return (sh_err2(NULL, path, strerror(errno)), 126);
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

	if (waitpid(pid, &status, 0) == -1)
		return (sh_sig_mode(SIG_INTERACTIVE), 1);
	sh_sig_mode(SIG_INTERACTIVE);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	rn_exec_ext(char **args, t_env **env)
{
	char	*path;
	char	**envp;
	pid_t	pid;
	int		status;

	if (!args || !args[0])
		return (0);
	status = rn_ext_resolve(args, env, &path);
	if (status)
		return (free(path), status);
	envp = env_toarr(env);
	if (!envp)
		return (free(path), 1);
	sh_sig_mode(SIG_EXEC);
	pid = fork();
	if (pid == -1)
		return (free(path), sh_freeargs(envp), sh_sig_mode(SIG_INTERACTIVE), 1);
	if (pid == 0)
		rn_ext_child(path, args, envp);
	return (free(path), sh_freeargs(envp), rn_ext_wait(pid));
}
