#include "tester.h"

#define TEST_SHARED_BUILTIN
#include "shared.c"

describe(bin_echo)
{
	it("prints arguments with newline")
	{
		char	*args[] = {"hello", "world", NULL};
		t_env	*env;
		char	*out;
		int		code;

		env = NULL;
		out = capture_builtin(bin_echo, args, &env, &code);
		asserteq(code, 0);
		asserteq_str(out, "hello world\n");
		free(out);
	}

	it("supports -n")
	{
		char	*args[] = {"-n", "hello", NULL};
		t_env	*env;
		char	*out;
		int		code;

		env = NULL;
		out = capture_builtin(bin_echo, args, &env, &code);
		asserteq(code, 0);
		asserteq_str(out, "hello");
		free(out);
	}

	it("handles repeated -n flags")
	{
		char	*args[] = {"-n", "-nn", "-nnn", "hello", NULL};
		t_env	*env;
		char	*out;
		int		code;

		env = NULL;
		out = capture_builtin(bin_echo, args, &env, &code);
		asserteq(code, 0);
		asserteq_str(out, "hello");
		free(out);
	}
}

describe(bin_export)
{
	it("sets a value")
	{
		char	*args[] = {"FOO=bar", NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_export(args, &env), 0);
		asserteq_str(env_get(&env, "FOO"), "bar");
		env_free(&env);
	}

	it("creates a node without a value")
	{
		char	*args[] = {"KEY", NULL};
		t_env	*env;
		t_env	*node;

		env = NULL;
		asserteq(bin_export(args, &env), 0);
		node = find_env_node(env, "KEY");
		assert(node != NULL);
		assert(node->value == NULL);
		env_free(&env);
	}

	it("creates a node with an empty value")
	{
		char	*args[] = {"KEY=", NULL};
		t_env	*env;
		t_env	*node;

		env = NULL;
		asserteq(bin_export(args, &env), 0);
		node = find_env_node(env, "KEY");
		assert(node != NULL);
		asserteq_str(node->value, "");
		env_free(&env);
	}

	it("accepts underscores and digits after the first char")
	{
		char	*args[] = {"_A1=value", NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_export(args, &env), 0);
		asserteq_str(env_get(&env, "_A1"), "value");
		env_free(&env);
	}

	it("rejects unsupported append assignment syntax")
	{
		char	*args[] = {"A+=value", NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_export(args, &env), 1);
		assert(env_get(&env, "A") == NULL);
		env_free(&env);
	}

	it("rejects invalid identifiers")
	{
		char	*args[] = {"1BAD=value", NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_export(args, &env), 1);
		env_free(&env);
	}

	it("prints sorted declare -x output when no args")
	{
		char	*envp[] = {"Z=last", "A=first", NULL};
		char	*args[] = {NULL};
		t_env	*env;
		char	*out;
		int		code;

		env = env_init(envp);
		out = capture_builtin(bin_export, args, &env, &code);
		asserteq(code, 0);
		assert(out != NULL);
		assert(strstr(out, "declare -x A=\"first\"") != NULL);
		assert(strstr(out, "declare -x Z=\"last\"") != NULL);
		assert(strstr(out, "declare -x A") < strstr(out, "declare -x Z"));
		free(out);
		env_free(&env);
	}
}

describe(bin_unset)
{
	it("removes exported values")
	{
		char	*envp[] = {"A=1", NULL};
		char	*args[] = {"A", NULL};
		t_env	*env;

		env = env_init(envp);
		asserteq(bin_unset(args, &env), 0);
		assert(env_get(&env, "A") == NULL);
		env_free(&env);
	}

	it("ignores missing keys")
	{
		char	*envp[] = {"A=1", NULL};
		char	*args[] = {"MISSING", NULL};
		t_env	*env;

		env = env_init(envp);
		asserteq(bin_unset(args, &env), 0);
		asserteq_str(env_get(&env, "A"), "1");
		env_free(&env);
	}
}

describe(bin_env)
{
	it("prints only entries with values")
	{
		char	*envp[] = {"A=1", "B", NULL};
		char	*args[] = {NULL};
		t_env	*env;
		char	*out;
		int		code;

		env = env_init(envp);
		out = capture_builtin(bin_env, args, &env, &code);
		asserteq(code, 0);
		assert(strstr(out, "A=1\n") != NULL);
		assert(strstr(out, "B=") == NULL);
		free(out);
		env_free(&env);
	}
}

describe(bin_pwd)
{
	it("returns success when cwd is available")
	{
		char	*args[] = {NULL};
		t_env	*env;
		char	*out;
		int		code;

		env = NULL;
		out = capture_builtin(bin_pwd, args, &env, &code);
		asserteq(code, 0);
		assert(out[0] == '/');
		free(out);
	}
}

describe(bin_exit)
{
	it("returns 1 for too many args without exiting")
	{
		char	*args[] = {"1", "2", NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_exit(args, &env), 1);
		env_free(&env);
	}

	it("exits with 2 for non-numeric argument")
	{
		char	*args[] = {"notanumber", NULL};
		t_env	*env;

		env = NULL;
		asserteq(fork_exit(args, &env), 2);
		env_free(&env);
	}

	it("exits with the given numeric code")
	{
		char	*args[] = {"42", NULL};
		t_env	*env;

		env = NULL;
		asserteq(fork_exit(args, &env), 42);
		env_free(&env);
	}

	it("exits with 2 for numeric overflow")
	{
		char	*args[] = {"9223372036854775808", NULL};
		t_env	*env;

		env = NULL;
		asserteq(fork_exit(args, &env), 2);
		env_free(&env);
	}
}

describe(bin_cd)
{
	it("changes to HOME when no argument given")
	{
		char	cwd[4096];
		char	*envp[] = {"HOME=tests/_tmp", NULL};
		char	*args[] = {NULL};
		t_env	*env;

		assert(getcwd(cwd, sizeof(cwd)) != NULL);
		ensure_tmp_dir();
		env = env_init(envp);
		asserteq(bin_cd(args, &env), 0);
		assert(strstr(env_get(&env, "PWD"), "/tests/_tmp") != NULL);
		chdir(cwd);
		env_free(&env);
	}

	it("returns 1 when HOME is not set")
	{
		char	*args[] = {NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_cd(args, &env), 1);
		env_free(&env);
	}

	it("changes to a given path and updates PWD")
	{
		char	cwd[4096];
		char	*args[] = {"tests/_tmp", NULL};
		t_env	*env;

		assert(getcwd(cwd, sizeof(cwd)) != NULL);
		ensure_tmp_dir();
		env = NULL;
		asserteq(bin_cd(args, &env), 0);
		assert(strstr(env_get(&env, "PWD"), "/tests/_tmp") != NULL);
		chdir(cwd);
		env_free(&env);
	}

	it("updates OPWD as well as PWD")
	{
		char	cwd[4096];
		char	*args[] = {"tests", NULL};
		t_env	*env;

		assert(getcwd(cwd, sizeof(cwd)) != NULL);
		env = NULL;
		asserteq(env_set(&env, "PWD", cwd), 0);
		asserteq(bin_cd(args, &env), 0);
		asserteq_str(env_get(&env, ENV_OPWD), cwd);
		assert(env_get(&env, "PWD") != NULL);
		assert(strstr(env_get(&env, "PWD"), "/tests") != NULL);
		chdir(cwd);
		env_free(&env);
	}

	it("returns 1 for a missing path")
	{
		char	*args[] = {"tests/_tmp/__minishell_cd_missing__", NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_cd(args, &env), 1);
		env_free(&env);
	}

	it("cd - goes to OPWD and prints it")
	{
		char	cwd[4096];
		char	*args[] = {"-", NULL};
		char	*envp[] = {"OPWD=tests/_tmp", NULL};
		t_env	*env;
		char	*out;
		int		code;

		assert(getcwd(cwd, sizeof(cwd)) != NULL);
		ensure_tmp_dir();
		env = env_init(envp);
		out = capture_builtin(bin_cd, args, &env, &code);
		asserteq(code, 0);
		assert(strstr(env_get(&env, "PWD"), "/tests/_tmp") != NULL);
		assert(out != NULL);
		assert(strstr(out, "tests/_tmp") != NULL);
		chdir(cwd);
		free(out);
		env_free(&env);
	}

	it("cd - returns 1 when OPWD is not set")
	{
		char	*args[] = {"-", NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_cd(args, &env), 1);
		env_free(&env);
	}
}

snow_main();
