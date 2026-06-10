/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 17:14:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

static int	redir_stdio(t_parser_redir_type type)
{
	if (type == REDIR_OUT || type == REDIR_APPEND)
		return (STDOUT_FILENO);
	return (STDIN_FILENO);
}

static void	redir_close_opened(t_redir_fd *opened, size_t count)
{
	while (count > 0)
		close(opened[--count].fd);
}

static int	redir_open_all(t_redir_fd *opened, t_redir_ctx *ctx)
{
	size_t	i;
	int		fd;

	i = 0;
	while (i < ctx->count)
	{
		fd = rn_redir_fd(&ctx->redirs[i], ctx->env, ctx->input_fd);
		if (fd < 0)
		{
			redir_close_opened(opened, i);
			if (fd == REDIR_FD_INTR)
				return (EXIT_SIGINT);
			return (1);
		}
		opened[i].fd = fd;
		opened[i].stdio = redir_stdio(ctx->redirs[i].type);
		i++;
	}
	return (0);
}

static int	redir_dup_all(t_redir_fd *opened, size_t count)
{
	size_t	i;

	i = 0;
	while (i < count)
	{
		if (dup2(opened[i].fd, opened[i].stdio) == -1)
		{
			while (i < count)
				close(opened[i++].fd);
			sh_err(NULL, "dup2 failed");
			return (1);
		}
		close(opened[i].fd);
		i++;
	}
	return (0);
}

int	rn_redir_apply(t_redir_ctx *ctx)
{
	t_redir_fd	*opened;
	int			status;

	if (!ctx->redirs || !ctx->count)
		return (0);
	opened = malloc(sizeof(*opened) * ctx->count);
	if (!opened)
		return (1);
	status = redir_open_all(opened, ctx);
	if (status)
	{
		free(opened);
		return (status);
	}
	status = redir_dup_all(opened, ctx->count);
	free(opened);
	return (status);
}
