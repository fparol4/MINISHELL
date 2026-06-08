/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 00:00:00 by fcardozo          #+#    #+#             */
/*   Updated: 2026/06/08 00:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_pipe.h"

static int	rn_pipe_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

int	rn_pipe_wait(pid_t *pids, size_t count)
{
	size_t	i;
	int		status;
	int		last;

	i = 0;
	last = 1;
	while (i < count)
	{
		if (waitpid(pids[i], &status, 0) != -1 && i == count - 1)
			last = rn_pipe_status(status);
		i++;
	}
	return (last);
}

static int	rn_pipe_fork_all(t_command **cmds, t_env **env, int *fds,
	pid_t *pids, size_t pipe_count, size_t *forked)
{
	size_t	i;

	i = 0;
	while (cmds[i])
	{
		pids[i] = fork();
		if (pids[i] == -1)
			return (*forked = i, sh_err(NULL, "fork failed"), 1);
		if (pids[i] == 0)
			rn_pipe_child(cmds, env, fds, i, pipe_count);
		i++;
	}
	return (*forked = i, 0);
}

int	rn_pipe_fork_wait(t_command **cmds, t_env **env, int *fds, size_t count)
{
	pid_t	*pids;
	size_t	forked;
	int		status;

	pids = ft_calloc(count, sizeof(pid_t));
	if (!pids)
		return (1);
	sh_sig_mode(SIG_EXEC);
	forked = 0;
	status = rn_pipe_fork_all(cmds, env, fds, pids, count - 1, &forked);
	rn_pipe_close_all(fds, count - 1);
	if (status)
		rn_pipe_wait(pids, forked);
	else
		status = rn_pipe_wait(pids, count);
	sh_sig_mode(SIG_INTERACTIVE);
	return (free(pids), status);
}
