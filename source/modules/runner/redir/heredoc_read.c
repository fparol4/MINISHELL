/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 00:00:00 by fcardozo          #+#    #+#             */
/*   Updated: 2026/06/08 00:00:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

static int	rn_redir_delim(char *line, char *target)
{
	return (ft_strcmp(line, target) == 0);
}

void	rn_redir_warn(char *target)
{
	ft_putstr_fd(MSG_MINISHELL, STDERR_FILENO);
	ft_putstr_fd(" warning: here-document delimited by end-of-file (wanted `",
		STDERR_FILENO);
	ft_putstr_fd(target, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

char	*rn_redir_readline(void)
{
	char	*line;
	size_t	len;
	size_t	cap;
	ssize_t	end;

	ft_putstr_fd("> ", STDERR_FILENO);
	line = NULL;
	len = 0;
	cap = 0;
	end = read_heredoc_loop(&line, &len, &cap);
	if (end == -1)
		return (free(line), NULL);
	if (!line && end <= 0)
		return (NULL);
	if (rn_redir_linegrow(&line, &cap, len))
		return (free(line), NULL);
	line[len] = '\0';
	return (line);
}

static int	heredoc_write_line(int fd, char *line, t_env **env, int expand)
{
	char	*out;

	out = rn_redir_line(line, env, expand);
	free(line);
	if (!out)
		return (1);
	if (write(fd, out, ft_strlen(out)) < 0)
		return (free(out), 1);
	free(out);
	return (0);
}

t_heredoc_state	heredoc_read_loop(int fd, char *target, t_env **env, int expand)
{
	char	*line;

	line = rn_redir_readline();
	while (line)
	{
		if (g_signal == SIGINT)
			return (free(line), g_signal = 0, HEREDOC_INTR);
		if (rn_redir_delim(line, target))
			return (free(line), HEREDOC_DONE);
		if (heredoc_write_line(fd, line, env, expand))
			return (HEREDOC_FAIL);
		line = rn_redir_readline();
	}
	return (HEREDOC_EOF);
}
