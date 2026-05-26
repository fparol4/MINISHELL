#include "../../../headers/runner.h"
#include "../../../libraries/libft/lib/ft_gnline/get_next_line.h"
#include <fcntl.h>

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

static char	*rn_redir_value(char *line, int *idx, t_env **env)
{
	char	*key;
	char	*value;
	int		start;

	if (line[*idx + 1] == '?')
	{
		*idx += 2;
		value = env_get(env, ENV_ERRCODE);
		if (!value)
			return (ft_strdup("0"));
		return (ft_strdup(value));
	}
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
	size_t	len;

	len = ft_strlen(line);
	if (len && line[len - 1] == '\n')
		len--;
	return (ft_strlen(target) == len && ft_strncmp(line, target, len) == 0);
}

static char	*rn_redir_line(char *line, t_env **env, int expand)
{
	char	*out;
	size_t	len;

	len = ft_strlen(line);
	if (len && line[len - 1] == '\n')
		line[--len] = '\0';
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

static int	rn_redir_heredoc_fill(int fd, char *target, t_env **env, int expand)
{
	char	*line;
	char	*out;

	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (rn_redir_delim(line, target))
			return (free(line), 0);
		out = rn_redir_line(line, env, expand);
		free(line);
		if (!out || write(fd, out, ft_strlen(out)) < 0)
			return (free(out), 1);
		free(out);
		line = get_next_line(STDIN_FILENO);
	}
	return (0);
}

static int	rn_redir_heredoc(char *target, t_env **env, int expand)
{
	int		pfd[2];

	if (pipe(pfd) == -1)
		return (sh_err(NULL, "pipe failed"), -1);
	if (rn_redir_heredoc_fill(pfd[1], target, env, expand))
		return (close(pfd[0]), close(pfd[1]), sh_err(NULL, "heredoc failed"), -1);
	close(pfd[1]);
	return (pfd[0]);
}

static char	*rn_redir_target(t_redir *redir, t_env **env)
{
	char	*raw[2];
	char	**expanded;
	char	*target;

	raw[0] = redir->target;
	raw[1] = NULL;
	expanded = rn_expand(raw, env);
	if (!expanded)
		return (NULL);
	if (!expanded[0] || expanded[1])
		return (sh_err2(NULL, redir->target, "ambiguous redirect"),
			sh_freeargs(expanded), NULL);
	target = ft_strdup(expanded[0]);
	sh_freeargs(expanded);
	return (target);
}

static int	rn_redir_open(char *target, t_redir_type type)
{
	if (type == IN_F)
		return (open(target, O_RDONLY));
	if (type == OUT_T)
		return (open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	return (open(target, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

static int	rn_redir_fd(t_redir *redir, t_env **env)
{
	char	*target;
	int		fd;

	target = rn_redir_target(redir, env);
	if (!target)
		return (-1);
	if (redir->type == IN_H)
		fd = rn_redir_heredoc(target, env, redir->expand);
	else
		fd = rn_redir_open(target, redir->type);
	if (fd == -1 && redir->type != IN_H)
		sh_err2(NULL, target, "open failed");
	free(target);
	return (fd);
}

static int	rn_redir_apply(t_redir *redir, t_env **env)
{
	int	fd;
	int	stdio;

	while (redir)
	{
		fd = rn_redir_fd(redir, env);
		if (fd == -1)
			return (1);
		stdio = STDIN_FILENO;
		if (redir->type == OUT_T || redir->type == OUT_A)
			stdio = STDOUT_FILENO;
		if (dup2(fd, stdio) == -1)
			return (close(fd), sh_err(NULL, "dup2 failed"), 1);
		close(fd);
		redir = redir->next;
	}
	return (0);
}

int	rn_redir_restore(int saved[2])
{
	if (saved[0] != -1 && dup2(saved[0], STDIN_FILENO) == -1)
		return (close(saved[0]), close(saved[1]), sh_err(NULL, "dup2 failed"), 1);
	if (saved[1] != -1 && dup2(saved[1], STDOUT_FILENO) == -1)
		return (close(saved[0]), close(saved[1]), sh_err(NULL, "dup2 failed"), 1);
	if (saved[0] != -1)
		close(saved[0]);
	if (saved[1] != -1)
		close(saved[1]);
	return (0);
}

int	rn_redir_push(t_redir *redir, t_env **env, int saved[2])
{
	saved[0] = -1;
	saved[1] = -1;
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	if (saved[0] == -1 || saved[1] == -1)
		return (rn_redir_restore(saved), sh_err(NULL, "dup failed"), 1);
	if (rn_redir_apply(redir, env))
		return (rn_redir_restore(saved), 1);
	return (0);
}
