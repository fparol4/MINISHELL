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

describe(rn_exec_bin)
{
	it("handles builtin commands")
	{
		char	*args[] = {"export", "FOO=bar", NULL};
		t_env	*env;
		int		status;

		env = NULL;
		status = 42;
		asserteq(rn_exec_bin(args, &env, &status), 1);
		asserteq(status, 0);
		asserteq_str(env_get(&env, "FOO"), "bar");
		env_free(&env);
	}

	it("returns 0 for external commands")
	{
		char	*args[] = {"ls", NULL};
		t_env	*env;
		int		status;

		env = NULL;
		status = 42;
		asserteq(rn_exec_bin(args, &env, &status), 0);
		asserteq(status, 42);
	}
}

describe(rn_execute_cmd)
{
	it("expands args and runs builtin command")
	{
		char	*raw[] = {"export", "FOO=$USER", NULL};
		char	*envp[] = {"USER=bar", NULL};
		t_env	*env;
		t_exnode	node;

		env = env_init(envp);
		rn_test_node(&node, CMD, raw, NULL, NULL);
		asserteq(rn_execute(&node, &env), 0);
		asserteq_str(env_get(&env, "FOO"), "bar");
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		env_free(&env);
	}

	it("runs external commands")
	{
		char	*raw[] = {"true", NULL};
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env;
		t_exnode	node;

		env = env_init(envp);
		rn_test_node(&node, CMD, raw, NULL, NULL);
		asserteq(rn_execute(&node, &env), 0);
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		env_free(&env);
	}

	it("returns 127 and updates status for missing external commands")
	{
		char	*raw[] = {"not_a_real_command_xyz", NULL};
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env;
		t_exnode	node;

		env = env_init(envp);
		rn_test_node(&node, CMD, raw, NULL, NULL);
		asserteq(rn_execute(&node, &env), 127);
		asserteq_str(env_get(&env, ENV_ERRCODE), "127");
		env_free(&env);
	}
}
