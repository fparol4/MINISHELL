#include "tester.h"

describe(rn_expand)
{
	it("keeps single quoted variables literal")
	{
		char	*raw[] = {"'$USER'", NULL};
		char	*envp[] = {"USER=fcardozo", NULL};
		t_env	*env;
		char	**out;

		env = env_init(envp);
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "$USER");
		assert(out[1] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}

	it("expands variables inside double quotes without splitting")
	{
		char	*raw[] = {"\"hello $USER\"", NULL};
		char	*envp[] = {"USER=foo bar", NULL};
		t_env	*env;
		char	**out;

		env = env_init(envp);
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "hello foo bar");
		assert(out[1] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}

	it("splits unquoted expanded variables")
	{
		char	*raw[] = {"$USER", NULL};
		char	*envp[] = {"USER=foo bar", NULL};
		t_env	*env;
		char	**out;

		env = env_init(envp);
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "foo");
		asserteq_str(out[1], "bar");
		assert(out[2] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}

	it("joins mixed quoted and unquoted parts into one word")
	{
		char	*raw[] = {"pre\"$USER\"post", NULL};
		char	*envp[] = {"USER=mid", NULL};
		t_env	*env;
		char	**out;

		env = env_init(envp);
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "premidpost");
		assert(out[1] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}
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

describe(rn_execute)
{
	it("expands args and runs builtin command")
	{
		char	*raw[] = {"export", "FOO=$USER", NULL};
		char	*envp[] = {"USER=bar", NULL};
		t_env	*env;
		t_exnode	node;

		env = env_init(envp);
		ft_bzero(&node, sizeof(t_exnode));
		node.type = CMD;
		node.args = raw;
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
		ft_bzero(&node, sizeof(t_exnode));
		node.type = CMD;
		node.args = raw;
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
		ft_bzero(&node, sizeof(t_exnode));
		node.type = CMD;
		node.args = raw;
		asserteq(rn_execute(&node, &env), 127);
		asserteq_str(env_get(&env, ENV_ERRCODE), "127");
		env_free(&env);
	}
}

describe(rn_status)
{
	it("sets and gets the shell status")
	{
		t_env	*env;

		env = NULL;
		asserteq(rn_status_set(&env, 42), 0);
		asserteq(rn_status_get(&env), 42);
		env_free(&env);
	}
}

snow_main();
