/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 14:06:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/runner.h"
#include <fcntl.h>

typedef enum e_heredoc_state
{
	HEREDOC_DONE,
	HEREDOC_EOF,
	HEREDOC_INTR,
	HEREDOC_FAIL
}						t_heredoc_state;

typedef struct s_redir_fd
{
	int					fd;
	int					stdio;
}						t_redir_fd;

static int	rn_redir_append(char **buf, char *part)
{
	char	*next;

	next = ft_strjoin(*buf, part);
	free(*buf);
	*buf = next;
	return (!next);
}

static int	rn_redir_char(char **buf, char c)
{
	char	part[2];

	part[0] = c;
	part[1] = '\0';
	return (rn_redir_append(buf, part));
}

static char	*redir_value_special(int *idx, t_env **env)
{
	char	*value;

	*idx += 2;
	value = env_get(env, ENV_ERRCODE);
	if (!value)
		return (ft_strdup("0"));
	return (ft_strdup(value));
}

static char	*rn_redir_value(char *line, int *idx, t_env **env)
{
	char	*key;
	char	*value;
	int		start;

	if (line[*idx + 1] == '?')
		return (redir_value_special(idx, env));
	if (!sh_varstart(line[*idx + 1]))
		return ((*idx)++, ft_strdup("$"));
	start = ++(*idx);
	while (line[*idx] && sh_varchar(line[*idx]))
		(*idx)++;
	key = ft_substr(line, start, *idx - start);
	if (!key)
		return (NULL);
	value = env_get(env, key);
	free(key);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*rn_redir_expand_line(char *line, t_env **env)
{
	char	*out;
	char	*part;
	int		i;

	out = ft_strdup("");
	if (!out)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] != '$' && rn_redir_char(&out, line[i++]))
			return (free(out), NULL);
		else if (line[i] == '$')
		{
			part = rn_redir_value(line, &i, env);
			if (!part || rn_redir_append(&out, part))
				return (free(part), free(out), NULL);
			free(part);
		}
	}
	return (out);
}

static int	rn_redir_delim(char *line, char *target)
{
	return (ft_strcmp(line, target) == 0);
}

static char	*rn_redir_line(char *line, t_env **env, int expand)
{
	char	*out;

	if (expand)
		out = rn_redir_expand_line(line, env);
	else
		out = ft_strdup(line);
	if (!out)
		return (NULL);
	if (rn_redir_append(&out, "\n"))
		return (free(out), NULL);
	return (out);
}

static void	rn_redir_warn(char *target)
{
	ft_putstr_fd(MSG_MINISHELL, STDERR_FILENO);
	ft_putstr_fd(" warning: here-document delimited by end-of-file (wanted `",
		STDERR_FILENO);
	ft_putstr_fd(target, STDERR_FILENO);
	ft_putstr_fd("')\n", STDERR_FILENO);
}

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

static ssize_t	read_heredoc_loop(char **line, size_t *len, size_t *cap)
{
	char	c;
	ssize_t	readed;

	while (TRUE)
	{
		readed = read(STDIN_FILENO, &c, 1);
		if (readed <= 0)
			return (readed);
		if (c == '\n')
			return (1);
		if (rn_redir_linegrow(line, cap, *len))
			return (-1);
		(*line)[(*len)++] = c;
	}
}

static char	*rn_redir_readline(void)
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
	{
		free(out);
		return (1);
	}
	free(out);
	return (0);
}

static t_heredoc_state	heredoc_read_loop(int fd, char *target, t_env **env,
		int expand)
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

static int	rn_redir_heredoc(char *target, t_env **env, int expand)
{
	int				pfd[2];
	t_heredoc_state	state;

	if (pipe(pfd) == -1)
		return (sh_err(NULL, "pipe failed"), -1);
	state = rn_redir_heredoc_fill(pfd[1], target, env, expand);
	if (state == HEREDOC_INTR)
		return (close(pfd[0]), close(pfd[1]), -2);
	if (state == HEREDOC_FAIL)
		return (close(pfd[0]), close(pfd[1]), sh_err(NULL, "heredoc failed"),
			-1);
	if (state == HEREDOC_EOF)
		rn_redir_warn(target);
	close(pfd[1]);
	return (pfd[0]);
}

static char	*rn_redir_target(t_parser_redir *redir, t_env **env)
{
	char	*raw[2];
	char	**expanded;
	char	*target;

	if (redir->type == REDIR_HEREDOC)
		return (sh_quote_remove(redir->file));
	raw[0] = redir->file;
	raw[1] = NULL;
	expanded = rn_expand(raw, env);
	if (!expanded)
		return (NULL);
	if (!expanded[0] || expanded[1])
		return (sh_err2(NULL, redir->file, "ambiguous redirect"),
			sh_freeargs(expanded), NULL);
	target = ft_strdup(expanded[0]);
	sh_freeargs(expanded);
	return (target);
}

static int	rn_redir_open(char *target, t_parser_redir_type type)
{
	if (type == REDIR_IN)
		return (open(target, O_RDONLY));
	if (type == REDIR_OUT)
		return (open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	return (open(target, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

static int	rn_redir_fd(t_parser_redir *redir, t_env **env)
{
	char	*target;
	int		fd;

	target = rn_redir_target(redir, env);
	if (!target)
		return (-1);
	if (redir->type == REDIR_HEREDOC)
		fd = rn_redir_heredoc(target, env, redir->expand);
	else
		fd = rn_redir_open(target, redir->type);
	if (fd == -1 && redir->type != REDIR_HEREDOC)
		sh_err2(NULL, target, "open failed");
	free(target);
	return (fd);
}

static int	redir_open_all(t_redir_fd *opened, t_parser_redir *redirs,
		size_t count, t_env **env)
{
	size_t	i;
	int		fd;

	i = 0;
	while (i < count)
	{
		fd = rn_redir_fd(&redirs[i], env);
		if (fd < 0)
		{
			while (i > 0)
				close(opened[--i].fd);
			if (fd == -2)
				return (130);
			return (1);
		}
		opened[i].fd = fd;
		opened[i].stdio = STDIN_FILENO;
		if (redirs[i].type == REDIR_OUT || redirs[i].type == REDIR_APPEND)
			opened[i].stdio = STDOUT_FILENO;
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
			return (sh_err(NULL, "dup2 failed"), 1);
		}
		close(opened[i].fd);
		i++;
	}
	return (0);
}

static int	rn_redir_apply(t_parser_redir *redirs, size_t count,
		t_env **env)
{
	t_redir_fd		*opened;
	int				status;

	if (!redirs || !count)
		return (0);
	opened = malloc(sizeof(*opened) * count);
	if (!opened)
		return (1);
	status = redir_open_all(opened, redirs, count, env);
	if (status)
		return (free(opened), status);
	status = redir_dup_all(opened, count);
	free(opened);
	return (status);
}

int	rn_redir_restore(int saved[2])
{
	if (saved[0] != -1 && dup2(saved[0], STDIN_FILENO) == -1)
		return (close(saved[0]), close(saved[1]), sh_err(NULL, "dup2 failed"),
			1);
	if (saved[1] != -1 && dup2(saved[1], STDOUT_FILENO) == -1)
		return (close(saved[0]), close(saved[1]), sh_err(NULL, "dup2 failed"),
			1);
	if (saved[0] != -1)
		close(saved[0]);
	if (saved[1] != -1)
		close(saved[1]);
	return (0);
}

int	rn_redir_push(t_parser_redir *redirs, size_t count,
		t_env **env, int saved[2])
{
	int	status;

	saved[0] = -1;
	saved[1] = -1;
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	if (saved[0] == -1 || saved[1] == -1)
		return (rn_redir_restore(saved), sh_err(NULL, "dup failed"), 1);
	if (fcntl(saved[0], F_SETFD, FD_CLOEXEC) == -1 || fcntl(saved[1], F_SETFD,
			FD_CLOEXEC) == -1)
		return (rn_redir_restore(saved), sh_err(NULL, "fcntl failed"), 1);
	status = rn_redir_apply(redirs, count, env);
	if (status)
		return (rn_redir_restore(saved), status);
	return (0);
}
