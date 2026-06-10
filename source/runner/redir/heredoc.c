/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:34:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 16:34:16 by fcardozo         ###   ########.fr       */
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

static t_heredoc_state	rn_redir_heredoc_fill(int fd, char *target, t_env **env,
		int expand, int input_fd)
{
	t_heredoc_state	status;

	g_signal = 0;
	sh_sig_mode(SIG_HEREDOC);
	status = rn_redir_heredoc_loop(fd, target, env, expand, input_fd);
	sh_sig_mode(SIG_INTERACTIVE);
	if (status == HEREDOC_EOF && g_signal == SIGINT)
	{
		g_signal = 0;
		return (HEREDOC_INTR);
	}
	return (status);
}

int	rn_redir_heredoc(char *target, t_env **env, int expand, int input_fd)
{
	int				pfd[2];
	t_heredoc_state	state;

	if (pipe(pfd) == -1)
	{
		sh_err(NULL, "pipe failed");
		return (REDIR_FD_ERR);
	}
	state = rn_redir_heredoc_fill(pfd[1], target, env, expand, input_fd);
	close(pfd[1]);
	if (state == HEREDOC_INTR)
	{
		close(pfd[0]);
		return (REDIR_FD_INTR);
	}
	if (state == HEREDOC_FAIL)
	{
		sh_err(NULL, "heredoc failed");
		close(pfd[0]);
		return (REDIR_FD_ERR);
	}
	if (state == HEREDOC_EOF)
		rn_redir_warn(target);
	return (pfd[0]);
}
