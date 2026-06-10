/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:34:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 16:34:16 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/parser.h"
#include "_pipe.h"
#include <readline/history.h>

static void	pipe_child_exit(int *fds, size_t pipe_count)
{
	rn_pipe_close_all(fds, pipe_count);
	sh_err(NULL, "dup2 failed");
	_exit(1);
}

/* Save the terminal fd before dup2 replaces stdin with the pipe read-end,
   so heredoc can still read from the terminal inside a pipeline. */
static int	rn_heredoc_fd(t_pipe_ctx *ctx, size_t pos)
{
	int	heredoc_fd;

	if (pos == 0)
		return (STDIN_FILENO);
	heredoc_fd = dup(STDIN_FILENO);
	if (heredoc_fd == -1)
		pipe_child_exit(ctx->fds, ctx->pipe_count);
	return (heredoc_fd);
}

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
		{
			rn_pipe_close_all(fds, pipe_count);
			sh_err(NULL, "pipe failed");
			return (1);
		}
		i++;
	}
	return (0);
}

static void	rn_pipe_child_cleanup(t_pipe_ctx *ctx)
{
	env_free(ctx->env);
	rl_clear_history();
	if (ctx->ast)
		parser_free_ast(ctx->ast);
	else
		parser_free_cmd(ctx->root);
	free(ctx->cmds);
	free(ctx->fds);
	free(ctx->pids);
}

void	rn_pipe_child(t_pipe_ctx *ctx, size_t pos)
{
	int	heredoc_fd;
	int	status;

	heredoc_fd = rn_heredoc_fd(ctx, pos);
	if (pos > 0 && dup2(ctx->fds[(pos - 1) * 2], STDIN_FILENO) == -1)
		pipe_child_exit(ctx->fds, ctx->pipe_count);
	if (ctx->cmds[pos + 1] && dup2(ctx->fds[pos * 2 + 1], STDOUT_FILENO) == -1)
		pipe_child_exit(ctx->fds, ctx->pipe_count);
	rn_pipe_close_all(ctx->fds, ctx->pipe_count);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	status = rn_execute(ctx->cmds[pos], ctx->env, heredoc_fd, NULL);
	rn_pipe_child_cleanup(ctx);
	_exit(status);
}
