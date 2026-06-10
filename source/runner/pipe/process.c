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

#include "_pipe.h"

static int	rn_pipe_status(int status)
{
	int	exit_status;

	if (WIFEXITED(status))
	{
		exit_status = WEXITSTATUS(status);
		return (exit_status);
	}
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	rn_pipe_wait(pid_t *pids, size_t count)
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

static int	rn_pipe_fork_all(t_pipe_ctx *ctx, pid_t *pids, size_t *forked)
{
	size_t	i;

	i = 0;
	while (ctx->cmds[i])
	{
		pids[i] = fork();
		if (pids[i] == -1)
		{
			*forked = i;
			sh_err(NULL, "fork failed");
			return (1);
		}
		if (pids[i] == 0)
			rn_pipe_child(ctx, i);
		i++;
	}
	*forked = i;
	return (0);
}

int	rn_pipe_fork_wait(t_command **cmds, t_env **env, int *fds, size_t count)
{
	t_pipe_ctx	ctx;
	pid_t		*pids;
	size_t		forked;
	int			status;

	ctx.cmds = cmds;
	ctx.env = env;
	ctx.fds = fds;
	ctx.pipe_count = count - 1;
	pids = ft_calloc(count, sizeof(pid_t));
	if (!pids)
		return (1);
	ctx.pids = pids;
	sh_sig_mode(SIG_EXEC);
	forked = 0;
	status = rn_pipe_fork_all(&ctx, pids, &forked);
	rn_pipe_close_all(fds, count - 1);
	if (status)
		rn_pipe_wait(pids, forked);
	else
		status = rn_pipe_wait(pids, count);
	sh_sig_mode(SIG_INTERACTIVE);
	free(pids);
	return (status);
}
