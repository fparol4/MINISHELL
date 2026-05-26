#include "tester.h"

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

	it("rejects invalid identifiers")
	{
		char	*args[] = {"1BAD=value", NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_export(args, &env), 1);
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
}

snow_main();
