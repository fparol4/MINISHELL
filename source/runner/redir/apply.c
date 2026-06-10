/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:34:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 16:34:16 by fcardozo         ###   ########.fr       */
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

static int	redir_open_all(t_redir_fd *opened, t_parser_redir *redirs,
		size_t count, t_env **env, int input_fd)
{
	size_t	i;
	int		fd;

	i = 0;
	while (i < count)
	{
		fd = rn_redir_fd(&redirs[i], env, input_fd);
		if (fd < 0)
		{
			redir_close_opened(opened, i);
			if (fd == REDIR_FD_INTR)
				return (EXIT_SIGINT);
			return (1);
		}
		opened[i].fd = fd;
		opened[i].stdio = redir_stdio(redirs[i].type);
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

int	rn_redir_apply(t_parser_redir *redirs, size_t count, t_env **env,
		int input_fd)
{
	t_redir_fd	*opened;
	int			status;

	if (!redirs || !count)
		return (0);
	opened = malloc(sizeof(*opened) * count);
	if (!opened)
		return (1);
	status = redir_open_all(opened, redirs, count, env, input_fd);
	if (status)
	{
		free(opened);
		return (status);
	}
	status = redir_dup_all(opened, count);
	free(opened);
	return (status);
}
