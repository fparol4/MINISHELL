/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   buffer.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:57 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:57 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

int	rn_redir_append(char **buf, char *part)
{
	char	*next;

	next = ft_strjoin(*buf, part);
	free(*buf);
	*buf = next;
	return (!next);
}

int	rn_redir_char(char **buf, char c)
{
	char	part[2];

	part[0] = c;
	part[1] = '\0';
	return (rn_redir_append(buf, part));
}

int	rn_redir_linegrow(char **line, size_t *cap, size_t len)
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
	return (free(*line), *line = next, 0);
}

ssize_t	read_heredoc_loop(int input_fd, char **line, size_t *len, size_t *cap)
{
	char	c;
	ssize_t	readed;

	while (TRUE)
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
