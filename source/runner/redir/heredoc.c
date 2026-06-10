/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 17:14:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

static void	rn_redir_warn(char *target)
{
	ft_putstr_fd("minishell:", STDERR_FILENO);
	ft_putstr_fd(" warning: here-document delimited by end-of-file (wanted `",
		STDERR_FILENO);
	ft_putstr_fd(target, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

static t_heredoc_state	rn_redir_heredoc_fill(t_heredoc_ctx *ctx)
{
	t_heredoc_state	status;

	g_signal = 0;
	sh_sig_mode(SIG_HEREDOC);
	status = rn_redir_heredoc_loop(ctx);
	sh_sig_mode(SIG_INTERACTIVE);
	if (status == HEREDOC_EOF && g_signal == SIGINT)
	{
		g_signal = 0;
		return (HEREDOC_INTR);
	}
	return (status);
}

static int	rn_redir_heredoc_status(t_heredoc_state state, int fd, char *target)
{
	if (state == HEREDOC_INTR)
	{
		close(fd);
		return (REDIR_FD_INTR);
	}
	if (state == HEREDOC_FAIL)
	{
		sh_err(NULL, "heredoc failed");
		close(fd);
		return (REDIR_FD_ERR);
	}
	if (state == HEREDOC_EOF)
		rn_redir_warn(target);
	return (fd);
}

int	rn_redir_heredoc(char *target, t_env **env, int expand, int input_fd)
{
	int				pfd[2];
	t_heredoc_ctx	ctx;
	t_heredoc_state	state;

	if (pipe(pfd) == -1)
	{
		sh_err(NULL, "pipe failed");
		return (REDIR_FD_ERR);
	}
	ctx.fd = pfd[1];
	ctx.target = target;
	ctx.env = env;
	ctx.expand = expand;
	ctx.input_fd = input_fd;
	state = rn_redir_heredoc_fill(&ctx);
	close(pfd[1]);
	return (rn_redir_heredoc_status(state, pfd[0], target));
}
