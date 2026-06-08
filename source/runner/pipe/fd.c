/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:15 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:15 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_pipe.h"

void	rn_pipe_close_all(int *fds, size_t pipe_count)
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

int	rn_pipe_create(int *fds, size_t pipe_count)
{
	size_t	i;

	i = 0;
	while (i < pipe_count * 2)
		fds[i++] = -1;
	i = 0;
	while (i < pipe_count)
	{
		if (pipe(&fds[i * 2]) == -1)
			return (rn_pipe_close_all(fds, pipe_count), sh_err(NULL,
					"pipe failed"), 1);
		i++;
	}
	return (0);
}

static void	pipe_child_exit(int *fds, size_t pipe_count)
{
	rn_pipe_close_all(fds, pipe_count);
	sh_err(NULL, "dup2 failed");
	_exit(1);
}

void	rn_pipe_child(t_pipe_ctx *ctx, size_t pos)
{
	if (pos > 0 && dup2(ctx->fds[(pos - 1) * 2], STDIN_FILENO) == -1)
		pipe_child_exit(ctx->fds, ctx->pipe_count);
	if (ctx->cmds[pos + 1] && dup2(ctx->fds[pos * 2 + 1], STDOUT_FILENO) == -1)
		pipe_child_exit(ctx->fds, ctx->pipe_count);
	rn_pipe_close_all(ctx->fds, ctx->pipe_count);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	_exit(rn_execute(ctx->cmds[pos], ctx->env));
}
