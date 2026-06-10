/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 17:14:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_pipe.h"

static int	pipe_alloc(t_command ***cmds, int **fds, size_t count)
{
	*cmds = ft_calloc(count + 1, sizeof(t_command *));
	*fds = malloc(sizeof(int) * (count - 1) * 2);
	if (!*cmds || !*fds)
	{
		free(*cmds);
		free(*fds);
		return (1);
	}
	return (0);
}

static int	rn_pipe_ctx(t_pipe_ctx *ctx, t_command *node, t_env **env,
		t_ast *ast)
{
	size_t	count;
	size_t	idx;

	count = rn_pipe_count(node);
	if (count < 2)
	{
		sh_err(NULL, "invalid pipe node");
		return (1);
	}
	if (pipe_alloc(&ctx->cmds, &ctx->fds, count))
		return (1);
	idx = 0;
	rn_pipe_flatten(node, ctx->cmds, &idx);
	if (rn_pipe_create(ctx->fds, count - 1))
	{
		free(ctx->cmds);
		free(ctx->fds);
		return (1);
	}
	ctx->root = node;
	ctx->ast = ast;
	ctx->env = env;
	ctx->pids = NULL;
	ctx->pipe_count = count - 1;
	return (0);
}

int	rn_pipe(t_command *node, t_env **env, t_ast *ast)
{
	t_pipe_ctx	ctx;
	int			status;

	if (rn_pipe_ctx(&ctx, node, env, ast))
		return (1);
	status = rn_pipe_fork_wait(&ctx);
	free(ctx.cmds);
	free(ctx.fds);
	return (status);
}
