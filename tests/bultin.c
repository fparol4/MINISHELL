#include "tester.h"

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
}

describe(bin_cd)
{
	it("changes to HOME when no argument given")
	{
		char	cwd[4096];
		char	*envp[] = {"HOME=/tmp", NULL};
		char	*args[] = {NULL};
		t_env	*env;

		assert(getcwd(cwd, sizeof(cwd)) != NULL);
		env = env_init(envp);
		asserteq(bin_cd(args, &env), 0);
		asserteq_str(env_get(&env, "PWD"), "/tmp");
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
		char	*args[] = {"/tmp", NULL};
		t_env	*env;

		assert(getcwd(cwd, sizeof(cwd)) != NULL);
		env = NULL;
		asserteq(bin_cd(args, &env), 0);
		asserteq_str(env_get(&env, "PWD"), "/tmp");
		chdir(cwd);
		env_free(&env);
	}

	it("returns 1 for a missing path")
	{
		char	*args[] = {"/tmp/__minishell_cd_missing__", NULL};
		t_env	*env;

		env = NULL;
		asserteq(bin_cd(args, &env), 1);
		env_free(&env);
	}

	it("cd - goes to OPWD and prints it")
	{
		char	cwd[4096];
		char	*args[] = {"-", NULL};
		char	*envp[] = {"OPWD=/tmp", NULL};
		t_env	*env;
		char	*out;
		int		code;

		assert(getcwd(cwd, sizeof(cwd)) != NULL);
		env = env_init(envp);
		out = capture_builtin(bin_cd, args, &env, &code);
		asserteq(code, 0);
		asserteq_str(env_get(&env, "PWD"), "/tmp");
		assert(out != NULL);
		assert(strstr(out, "/tmp") != NULL);
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
