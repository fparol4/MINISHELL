/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
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

void	rn_pipe_child_cleanup(t_pipe_ctx *ctx)
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
