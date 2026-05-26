#include "../tester.h"

static void	rn_test_node(t_exnode *node, t_node_type type, char **args,
		t_exnode *left, t_exnode *right)
{
	ft_bzero(node, sizeof(*node));
	node->type = type;
	node->args = args;
	node->left = left;
	node->right = right;
}

static char	*rn_test_capture_execute(t_exnode *node, t_env **env, int *code)
{
	char	buf[4096];
	char	*out;
	int		pfd[2];
	int		saved;
	ssize_t	n;

	if (pipe(pfd) == -1)
		return (NULL);
	saved = dup(STDOUT_FILENO);
	dup2(pfd[1], STDOUT_FILENO);
	close(pfd[1]);
	*code = rn_execute(node, env);
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

static char	*rn_test_capture_fd(int target, t_exnode *node, t_env **env,
		int *code)
{
	char	buf[4096];
	char	*out;
	int		pfd[2];
	int		saved;
	ssize_t	n;

	if (pipe(pfd) == -1)
		return (NULL);
	saved = dup(target);
	if (saved == -1)
		return (close(pfd[0]), close(pfd[1]), NULL);
	dup2(pfd[1], target);
	close(pfd[1]);
	*code = rn_execute(node, env);
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

static char	*rn_test_capture_error(t_exnode *node, t_env **env, int *code)
{
	return (rn_test_capture_fd(STDERR_FILENO, node, env, code));
}

static void	rn_test_redir(t_redir *redir, t_redir_type type, char *target,
		int expand)
{
	redir->type = type;
	redir->target = target;
	redir->expand = expand;
	redir->next = NULL;
}

static int	rn_test_with_stdin(char *input, t_exnode *node, t_env **env,
		int *status, char **out)
{
	int		pfd[2];
	int		saved;

	if (pipe(pfd) == -1)
		return (1);
	if (write(pfd[1], input, ft_strlen(input)) < 0)
		return (close(pfd[0]), close(pfd[1]), 1);
	close(pfd[1]);
	saved = dup(STDIN_FILENO);
	if (saved == -1)
		return (close(pfd[0]), 1);
	dup2(pfd[0], STDIN_FILENO);
	close(pfd[0]);
	*out = rn_test_capture_execute(node, env, status);
	dup2(saved, STDIN_FILENO);
	close(saved);
	return (0);
}

static int	rn_test_with_stdin_capture(char *input, t_exnode *node, t_env **env,
		int *status, char **captured, int target)
{
	int		pfd[2];
	int		saved;

	if (pipe(pfd) == -1)
		return (1);
	if (write(pfd[1], input, ft_strlen(input)) < 0)
		return (close(pfd[0]), close(pfd[1]), 1);
	close(pfd[1]);
	saved = dup(STDIN_FILENO);
	if (saved == -1)
		return (close(pfd[0]), 1);
	dup2(pfd[0], STDIN_FILENO);
	close(pfd[0]);
	*captured = rn_test_capture_fd(target, node, env, status);
	dup2(saved, STDIN_FILENO);
	close(saved);
	return (0);
}

static int	rn_test_with_stdin_error(char *input, t_exnode *node, t_env **env,
		int *status, char **err)
{
	return (rn_test_with_stdin_capture(input, node, env, status, err,
			STDERR_FILENO));
}

static int	rn_test_temp(char *tpl)
{
	return (mkstemp(tpl));
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
