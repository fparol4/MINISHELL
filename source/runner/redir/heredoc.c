/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:57 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:57 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

static t_heredoc_state	rn_redir_heredoc_fill(int fd, char *target, t_env **env,
		int expand)
{
	t_heredoc_state	status;

	g_signal = 0;
	sh_sig_mode(SIG_HEREDOC);
	status = heredoc_read_loop(fd, target, env, expand);
	sh_sig_mode(SIG_INTERACTIVE);
	if (status == HEREDOC_EOF && g_signal == SIGINT)
	{
		g_signal = 0;
		return (HEREDOC_INTR);
	}
	return (status);
}

static int	heredoc_close_read(int pfd[2], int status)
{
	close(pfd[0]);
	return (status);
}

int	rn_redir_heredoc(char *target, t_env **env, int expand)
{
	int				pfd[2];
	t_heredoc_state	state;

	if (pipe(pfd) == -1)
		return (sh_err(NULL, "pipe failed"), -1);
	state = rn_redir_heredoc_fill(pfd[1], target, env, expand);
	close(pfd[1]);
	if (state == HEREDOC_INTR)
		return (heredoc_close_read(pfd, -2));
	if (state == HEREDOC_FAIL)
	{
		sh_err(NULL, "heredoc failed");
		return (heredoc_close_read(pfd, -1));
	}
	if (state == HEREDOC_EOF)
		rn_redir_warn(target);
	return (pfd[0]);
}
