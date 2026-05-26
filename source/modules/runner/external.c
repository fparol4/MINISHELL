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
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/wait.h>

typedef enum e_ext_kind
{
	EXT_READY,
	EXT_NOT_FOUND,
	EXT_DENIED,
	EXT_DIR
}	t_ext_kind;

static int	rn_ext_slash(char *cmd)
{
	return (cmd && ft_strchr(cmd, '/'));
}

static t_ext_kind	rn_ext_classify(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		if (errno == ENOENT || errno == ENOTDIR)
			return (EXT_NOT_FOUND);
		return (EXT_DENIED);
	}
	if (S_ISDIR(st.st_mode))
		return (EXT_DIR);
	if (access(path, X_OK) == -1)
		return (EXT_DENIED);
	return (EXT_READY);
}

static int	rn_ext_report(char *path, t_ext_kind kind)
{
	if (kind == EXT_NOT_FOUND)
		return (sh_err2(NULL, path, "No such file or directory"), 127);
	if (kind == EXT_DIR)
		return (sh_err2(NULL, path, "Is a directory"), 126);
	return (sh_err2(NULL, path, "Permission denied"), 126);
}

static int	rn_ext_execfail(char *path)
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

static int	rn_ext_resolve(char **args, t_env **env, char **path)
{
	t_ext_kind	kind;

	*path = NULL;
	if (rn_ext_slash(args[0]))
	{
		*path = ft_strdup(args[0]);
		if (!*path)
			return (1);
	}
	else
		*path = rn_path(args, env);
	if (!*path)
		return (sh_err2(NULL, args[0], "command not found"), 127);
	kind = rn_ext_classify(*path);
	if (kind == EXT_READY)
		return (0);
	return (rn_ext_report(*path, kind));
}

static void	rn_ext_child(char *path, char **args, char **envp)
{
	execve(path, args, envp);
	_exit(rn_ext_execfail(path));
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
	pid = fork();
	if (pid == -1)
		return (free(path), sh_freeargs(envp), 1);
	if (pid == 0)
		rn_ext_child(path, args, envp);
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
