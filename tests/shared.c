#include "tester.h"

#ifdef TEST_SHARED_LEXER

static t_manager	*lex(const char *input)
{
	return (lexer(input));
}

static t_list_token	*nth_token(t_manager *m, unsigned int n)
{
	t_node	*cur;

	cur = m->head;
	while (n-- && cur)
		cur = cur->next;
	if (!cur)
		return (NULL);
	return ((t_list_token *)cur->content);
}

static unsigned int	token_count(t_manager *m)
{
	t_node			*cur;
	unsigned int	n;

	n = 0;
	cur = m->head;
	while (cur)
	{
		n++;
		cur = cur->next;
	}
	return (n);
}

#endif

#ifdef TEST_SHARED_PARSER

static t_ast	*parse_input(const char *input)
{
	t_manager	*m;
	t_ast		*ast;

	m = lexer(input);
	if (!m)
		return (NULL);
	ast = parser_controller(m);
	lexer_free(m);
	return (ast);
}

#endif

#ifdef TEST_SHARED_BUILTIN

static int	fork_exit(char **args, t_env **env)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == 0)
	{
		dup2(open("/dev/null", O_WRONLY), STDOUT_FILENO);
		dup2(open("/dev/null", O_WRONLY), STDERR_FILENO);
		bin_exit(args, env);
		_exit(1);
	}
	if (pid == -1)
		return (-1);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (-1);
}

static t_env	*find_env_node(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strcmp(env->key, key) == 0)
			return (env);
		env = env->next;
	}
	return (NULL);
}

static void	ensure_tmp_dir(void)
{
	mkdir("tests/_tmp", 0777);
}

static char	*capture_builtin(int (*fn)(char **, t_env **), char **args,
		t_env **env, int *code)
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
	*code = fn(args, env);
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

#endif

#ifdef TEST_SHARED_RUNNER

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
		node->t_define.simple.args.capacity
			= node->t_define.simple.args.length + 1;
	}
	else
	{
		node->t_define.pipe.left = left;
		node->t_define.pipe.right = right;
	}
}

static char	*rn_test_capture_execute(t_command *node, t_env **env, int *code)
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

static char	*rn_test_capture_fd(int target, t_command *node, t_env **env,
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

static char	*rn_test_capture_error(t_command *node, t_env **env, int *code)
{
	return (rn_test_capture_fd(STDERR_FILENO, node, env, code));
}

static void	rn_test_redir(t_parser_redir *redir, t_parser_redir_type type,
		char *target, int expand)
{
	redir->type = type;
	redir->file = target;
	redir->expand = expand;
	redir->quoted = !expand;
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

static int	rn_test_with_stdin_capture(char *input, t_command *node, t_env **env,
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

static int	rn_test_with_stdin_error(char *input, t_command *node, t_env **env,
		int *status, char **err)
{
	return (rn_test_with_stdin_capture(input, node, env, status, err,
			STDERR_FILENO));
}

static void	rn_test_ensure_tmp(void)
{
	mkdir("tests/_tmp", 0777);
}

static int	rn_test_temp(char *tpl)
{
	rn_test_ensure_tmp();
	return (mkstemp(tpl));
}

static char	*rn_test_mkdtemp(char *tpl)
{
	rn_test_ensure_tmp();
	return (mkdtemp(tpl));
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

#endif
