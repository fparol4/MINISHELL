/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tester.h"

static void	rn_test_node(t_command *node, t_pnode_type type, char **args,
		t_command *left, t_command *right)
{
	ft_bzero(node, sizeof(*node));
	node->type = type;
	if (type == PNODE_CMD)
	{
		node->t_define.simple.args.items = args;
		node->t_define.simple.args.elem_size = sizeof(char *);
		while (args && args[node->t_define.simple.args.length])
			node->t_define.simple.args.length++;
		node->t_define.simple.args.capacity = node->t_define.simple.args.length
			+ 1;
	}
	else
	{
		node->t_define.pipe.left = left;
		node->t_define.pipe.right = right;
	}
}

static t_ast	*rn_test_ast(void)
{
	t_ast	*ast;

	ast = ft_calloc(1, sizeof(t_ast));
	return (ast);
}

static char	*rn_test_capture_execute(t_command *node, t_env **env, int *code)
{
	char	buf[4096];
	char	*out;
	t_ast	*ast;
	int		pfd[2];
	int		saved;
	ssize_t	n;

	ast = rn_test_ast();
	if (pipe(pfd) == -1)
		return (NULL);
	saved = dup(STDOUT_FILENO);
	dup2(pfd[1], STDOUT_FILENO);
	close(pfd[1]);
	*code = rn_execute(node, env, STDIN_FILENO, ast);
	dup2(saved, STDOUT_FILENO);
	close(saved);
	n = read(pfd[0], buf, sizeof(buf) - 1);
	close(pfd[0]);
	if (n < 0)
		n = 0;
	buf[n] = '\0';
	out = ft_strdup(buf);
	return (out);
}

static char	*rn_test_capture_fd(int target, t_command *node, t_env **env,
		int *code)
{
	char	buf[4096];
	char	*out;
	t_ast	*ast;
	int		pfd[2];
	int		saved;
	ssize_t	n;

	ast = rn_test_ast();
	if (pipe(pfd) == -1)
		return (NULL);
	saved = dup(target);
	if (saved == -1)
	{
		close(pfd[0]);
		close(pfd[1]);
		return (NULL);
	}
	dup2(pfd[1], target);
	close(pfd[1]);
	*code = rn_execute(node, env, STDIN_FILENO, ast);
	dup2(saved, target);
	close(saved);
	n = read(pfd[0], buf, sizeof(buf) - 1);
	close(pfd[0]);
	if (n < 0)
		n = 0;
	buf[n] = '\0';
	out = ft_strdup(buf);
	return (out);
}

static char	*rn_test_capture_error(t_command *node, t_env **env, int *code)
{
	char	*out;

	out = rn_test_capture_fd(STDERR_FILENO, node, env, code);
	return (out);
}

static void	rn_test_redir(t_parser_redir *redir, t_parser_redir_type type,
		char *target, int expand)
{
	redir->type = type;
	redir->file = target;
	redir->expand = expand;
}

static void	rn_test_attach_redirs(t_command *node, t_parser_redir *redirs,
		size_t count)
{
	node->t_define.simple.redirs.items = redirs;
	node->t_define.simple.redirs.length = count;
	node->t_define.simple.redirs.capacity = count;
	node->t_define.simple.redirs.elem_size = sizeof(t_parser_redir);
}

static int	rn_test_with_stdin(char *input, t_command *node, t_env **env,
		int *status, char **out)
{
	int	pfd[2];
	int	saved;

	if (pipe(pfd) == -1)
		return (1);
	if (write(pfd[1], input, ft_strlen(input)) < 0)
	{
		close(pfd[0]);
		close(pfd[1]);
		return (1);
	}
	close(pfd[1]);
	saved = dup(STDIN_FILENO);
	if (saved == -1)
	{
		close(pfd[0]);
		return (1);
	}
	dup2(pfd[0], STDIN_FILENO);
	close(pfd[0]);
	*out = rn_test_capture_execute(node, env, status);
	dup2(saved, STDIN_FILENO);
	close(saved);
	return (0);
}

static int	rn_test_with_stdin_capture(char *input, t_command *node,
		t_env **env, int *status, char **captured, int target)
{
	int	pfd[2];
	int	saved;

	if (pipe(pfd) == -1)
		return (1);
	if (write(pfd[1], input, ft_strlen(input)) < 0)
	{
		close(pfd[0]);
		close(pfd[1]);
		return (1);
	}
	close(pfd[1]);
	saved = dup(STDIN_FILENO);
	if (saved == -1)
	{
		close(pfd[0]);
		return (1);
	}
	dup2(pfd[0], STDIN_FILENO);
	close(pfd[0]);
	*captured = rn_test_capture_fd(target, node, env, status);
	dup2(saved, STDIN_FILENO);
	close(saved);
	return (0);
}

static int	rn_test_with_stdin_error(char *input, t_command *node, t_env **env,
		int *status, char **err)
{
	int	code;

	code = rn_test_with_stdin_capture(input, node, env, status, err,
			STDERR_FILENO);
	return (code);
}

static void	rn_test_ensure_tmp(void)
{
	mkdir("tests/_tmp", 0777);
}

static int	rn_test_temp(char *tpl)
{
	int	fd;

	rn_test_ensure_tmp();
	fd = mkstemp(tpl);
	return (fd);
}

static char	*rn_test_mkdtemp(char *tpl)
{
	char	*dir;

	rn_test_ensure_tmp();
	dir = mkdtemp(tpl);
	return (dir);
}

static char	*rn_test_readfile(char *path)
{
	char	buf[4096];
	char	*out;
	int		fd;
	ssize_t	n;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	n = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (n < 0)
		return (NULL);
	buf[n] = '\0';
	out = ft_strdup(buf);
	return (out);
}

static char	*rn_test_quote(char *path)
{
	char	*tmp;
	char	*out;

	tmp = ft_strjoin("\"", path);
	assert(tmp != NULL);
	out = ft_strjoin(tmp, "\"");
	free(tmp);
	assert(out != NULL);
	return (out);
}
