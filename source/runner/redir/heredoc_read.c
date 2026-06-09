/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_read.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:47 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:47 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

static int	rn_redir_linegrow(char **line, size_t *cap, size_t len)
{
	char	*next;

	if (len + 1 < *cap)
		return (0);
	if (!*cap)
		*cap = 32;
	else
		*cap *= 2;
	next = malloc(sizeof(char) * *cap);
	if (!next)
		return (1);
	if (*line)
		ft_memcpy(next, *line, len);
	free(*line);
	*line = next;
	return (0);
}

static ssize_t	rn_heredoc_read_loop(int input_fd, char **line, size_t *len,
		size_t *cap)
{
	char	c;
	ssize_t	readed;

	while (1)
	{
		readed = read(input_fd, &c, 1);
		if (readed <= 0)
			return (readed);
		if (c == '\n')
			return (1);
		if (rn_redir_linegrow(line, cap, *len))
			return (-1);
		(*line)[(*len)++] = c;
	}
}

static int	rn_redir_delim(char *line, char *target)
{
	int	is_delim;

	is_delim = (ft_strcmp(line, target) == 0);
	return (is_delim);
}

char	*rn_redir_readline(int input_fd)
{
	char	*line;
	size_t	len;
	size_t	cap;
	ssize_t	end;

	ft_putstr_fd("> ", STDERR_FILENO);
	line = NULL;
	len = 0;
	cap = 0;
	end = rn_heredoc_read_loop(input_fd, &line, &len, &cap);
	if (end == -1)
	{
		free(line);
		return (NULL);
	}
	if (!line && end <= 0)
		return (NULL);
	if (rn_redir_linegrow(&line, &cap, len))
	{
		free(line);
		return (NULL);
	}
	line[len] = '\0';
	return (line);
}

static int	rn_redir_write_line(int fd, char *line, t_env **env, int expand)
{
	char	*out;

	out = rn_redir_line(line, env, expand);
	free(line);
	if (!out)
		return (1);
	if (write(fd, out, ft_strlen(out)) < 0)
	{
		free(out);
		return (1);
	}
	free(out);
	return (0);
}

t_heredoc_state	rn_redir_heredoc_loop(int fd, char *target, t_env **env,
		int expand, int input_fd)
{
	char	*line;

	while (1)
	{
		line = rn_redir_readline(input_fd);
		if (!line)
			return (HEREDOC_EOF);
		if (g_signal == SIGINT)
		{
			free(line);
			g_signal = 0;
			return (HEREDOC_INTR);
		}
		if (rn_redir_delim(line, target))
		{
			free(line);
			return (HEREDOC_DONE);
		}
		if (rn_redir_write_line(fd, line, env, expand))
			return (HEREDOC_FAIL);
	}
}
