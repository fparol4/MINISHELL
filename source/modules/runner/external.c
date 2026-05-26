/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   external.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/runner.h"
#include <sys/wait.h>

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
