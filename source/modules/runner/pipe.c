/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 14:06:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/runner.h"
#include <sys/wait.h>

static int	rn_pipe_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static size_t	rn_pipe_count(t_command *node)
{
	if (!node)
		return (0);
	if (node->type == PNODE_PIPE)
		return (rn_pipe_count(node->t_define.pipe.left)
			+ rn_pipe_count(node->t_define.pipe.right));
	return (1);
}

static void	rn_pipe_flatten(t_command *node, t_command **items, size_t *idx)
{
	if (!node)
		return ;
	if (node->type == PNODE_PIPE)
	{
		rn_pipe_flatten(node->t_define.pipe.left, items, idx);
		rn_pipe_flatten(node->t_define.pipe.right, items, idx);
		return ;
	}
	items[(*idx)++] = node;
}

static void	rn_pipe_close_all(int *fds, size_t pipe_count)
{
	size_t	i;

	i = 0;
	while (i < pipe_count * 2)
	{
		if (fds[i] != -1)
			close(fds[i]);
		i++;
	}
}

static int	rn_pipe_create(int *fds, size_t pipe_count)
{
	size_t	i;

	i = 0;
	while (i < pipe_count * 2)
		fds[i++] = -1;
	i = 0;
	while (i < pipe_count)
	{
		if (pipe(&fds[i * 2]) == -1)
			return (rn_pipe_close_all(fds, pipe_count),
				sh_err(NULL, "pipe failed"), 1);
		i++;
	}
	return (0);
}

static void	rn_pipe_child(t_command **cmds, t_env **env, int *fds,
		size_t pos, size_t pipe_count)
{
	if (pos > 0 && dup2(fds[(pos - 1) * 2], STDIN_FILENO) == -1)
	{
		rn_pipe_close_all(fds, pipe_count);
		sh_err(NULL, "dup2 failed");
		_exit(1);
	}
	if (cmds[pos + 1] && dup2(fds[pos * 2 + 1], STDOUT_FILENO) == -1)
	{
		rn_pipe_close_all(fds, pipe_count);
		sh_err(NULL, "dup2 failed");
		_exit(1);
	}
	rn_pipe_close_all(fds, pipe_count);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	_exit(rn_execute(cmds[pos], env));
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
	*forked = i;
	return (0);
}

static int	pipe_fork_and_wait(t_command **cmds, t_env **env, int *fds,
		size_t count)
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
	free(pids);
	return (status);
}

int	rn_pipe(t_command *node, t_env **env)
{
	t_command	**cmds;
	int			*fds;
	size_t		count;
	size_t		idx;
	int			status;

	count = rn_pipe_count(node);
	if (count < 2)
		return (sh_err(NULL, "invalid pipe node"), 1);
	cmds = ft_calloc(count + 1, sizeof(t_command *));
	fds = malloc(sizeof(int) * (count - 1) * 2);
	if (!cmds || !fds)
		return (free(cmds), free(fds), 1);
	idx = 0;
	rn_pipe_flatten(node, cmds, &idx);
	if (rn_pipe_create(fds, count - 1))
		return (free(cmds), free(fds), 1);
	status = pipe_fork_and_wait(cmds, env, fds, count);
	free(cmds);
	free(fds);
	return (status);
}
