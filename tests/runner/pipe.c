#include "../tester.h"

static void	rn_test_pipe_node(t_exnode *node, t_node_type type, char **args,
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

describe(rn_pipe)
{
	it("pipes left stdout into right stdin")
	{
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"echo", "hello", NULL};
		char	*right_args[] = {"wc", "-c", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		char	*out;
		int		status;

		env = env_init(envp);
		rn_test_pipe_node(&left, CMD, left_args, NULL, NULL);
		rn_test_pipe_node(&right, CMD, right_args, NULL, NULL);
		rn_test_pipe_node(&node, PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "6\n");
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		free(out);
		env_free(&env);
	}

	it("returns the right-side status")
	{
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"not_a_real_command_xyz", NULL};
		char	*right_args[] = {"true", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		int		status;

		env = env_init(envp);
		rn_test_pipe_node(&left, CMD, left_args, NULL, NULL);
		rn_test_pipe_node(&right, CMD, right_args, NULL, NULL);
		rn_test_pipe_node(&node, PIPE, NULL, &left, &right);
		status = rn_execute(&node, &env);
		asserteq(status, 0);
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		env_free(&env);
	}
}
