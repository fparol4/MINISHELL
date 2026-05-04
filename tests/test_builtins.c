# include "snow.h"
# include "envm.h"
# include "builtins.h"
# include "libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>

/* helpers */
static char	*capture_stdout(int (*fn)(char **, t_env **),
				char **args, t_env **env)
{
	int		pfd[2];
	char	buf[4096];
	ssize_t	n;
	char	*out;

	pipe(pfd);
	int saved = dup(1);
	dup2(pfd[1], 1);
	close(pfd[1]);
	fn(args, env);
	fflush(NULL);
	dup2(saved, 1);
	close(saved);
	n = read(pfd[0], buf, sizeof(buf) - 1);
	close(pfd[0]);
	if (n < 0) n = 0;
	buf[n] = '\0';
	out = strdup(buf);
	return (out);
}

/* ------------------------------------------------------------------ echo -- */
describe(builtin_echo)
{
	it("prints arguments separated by spaces with newline")
	{
		t_env	*env = NULL;
		char	*args[] = {"echo", "hello", "world", NULL};
		char	*out = capture_stdout(builtin_echo, args, &env);
		asserteq_str(out, "hello world\n");
		free(out);
	}

	it("suppresses newline with -n")
	{
		t_env	*env = NULL;
		char	*args[] = {"echo", "-n", "hi", NULL};
		char	*out = capture_stdout(builtin_echo, args, &env);
		asserteq_str(out, "hi");
		free(out);
	}

	it("handles multiple -n flags")
	{
		t_env	*env = NULL;
		char	*args[] = {"echo", "-n", "-n", "hi", NULL};
		char	*out = capture_stdout(builtin_echo, args, &env);
		asserteq_str(out, "hi");
		free(out);
	}

	it("handles combined -nnn flag")
	{
		t_env	*env = NULL;
		char	*args[] = {"echo", "-nnn", "hi", NULL};
		char	*out = capture_stdout(builtin_echo, args, &env);
		asserteq_str(out, "hi");
		free(out);
	}

	it("prints nothing (just newline) with no args")
	{
		t_env	*env = NULL;
		char	*args[] = {"echo", NULL};
		char	*out = capture_stdout(builtin_echo, args, &env);
		asserteq_str(out, "\n");
		free(out);
	}
}

/* ------------------------------------------------------------------- cd -- */
describe(builtin_cd)
{
	it("changes directory and updates PWD")
	{
		char	*env_arr[] = {"PWD=/tmp", "HOME=/root", NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"cd", "/tmp", NULL};
		asserteq(builtin_cd(args, &env), 0);
		asserteq_str(envm_get(env, "PWD"), "/tmp");
		envm_free(&env);
	}

	it("sets OLDPWD to previous PWD")
	{
		char	*env_arr[] = {"PWD=/home", "HOME=/root", NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"cd", "/tmp", NULL};
		builtin_cd(args, &env);
		asserteq_str(envm_get(env, "OLDPWD"), "/home");
		envm_free(&env);
	}

	it("returns 1 on invalid path")
	{
		char	*env_arr[] = {"PWD=/tmp", "HOME=/root", NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"cd", "/this_does_not_exist_42", NULL};
		asserteq(builtin_cd(args, &env), 1);
		envm_free(&env);
	}

	it("uses HOME when no argument given")
	{
		char	*env_arr[] = {"PWD=/tmp", "HOME=/tmp", NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"cd", NULL};
		asserteq(builtin_cd(args, &env), 0);
		envm_free(&env);
	}
}

/* ----------------------------------------------------------------- unset -- */
describe(builtin_unset)
{
	it("removes an existing key")
	{
		char	*env_arr[] = {"FOO=bar", NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"unset", "FOO", NULL};
		builtin_unset(args, &env);
		assert(envm_get(env, "FOO") == NULL);
		envm_free(&env);
	}

	it("silently succeeds for non-existent key")
	{
		char	*env_arr[] = {"FOO=bar", NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"unset", "GHOST", NULL};
		asserteq(builtin_unset(args, &env), 0);
		envm_free(&env);
	}
}

/* --------------------------------------------------------------- export -- */
describe(builtin_export)
{
	it("inserts a new variable")
	{
		char	*env_arr[] = {NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"export", "MYVAR=hello", NULL};
		builtin_export(args, &env);
		asserteq_str(envm_get(env, "MYVAR"), "hello");
		envm_free(&env);
	}

	it("updates an existing variable")
	{
		char	*env_arr[] = {"MYVAR=old", NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"export", "MYVAR=new", NULL};
		builtin_export(args, &env);
		asserteq_str(envm_get(env, "MYVAR"), "new");
		envm_free(&env);
	}

	it("returns 1 for invalid identifier")
	{
		char	*env_arr[] = {NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"export", "1BADVAR=x", NULL};
		asserteq(builtin_export(args, &env), 1);
		envm_free(&env);
	}

	it("prints declare -x output with no args")
	{
		char	*env_arr[] = {"FOO=bar", NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"export", NULL};
		char	*out = capture_stdout(builtin_export, args, &env);
		assert(strstr(out, "declare -x FOO=\"bar\"") != NULL);
		free(out);
		envm_free(&env);
	}
}

/* ------------------------------------------------------------------- env -- */
describe(builtin_env)
{
	it("prints variables that have a value")
	{
		char	*env_arr[] = {"FOO=bar", NULL};
		t_env	*env = envm_init(env_arr);
		char	*args[] = {"env", NULL};
		char	*out = capture_stdout(builtin_env, args, &env);
		assert(strstr(out, "FOO=bar") != NULL);
		free(out);
		envm_free(&env);
	}

	it("does not print variables without a value")
	{
		char	*env_arr[] = {NULL};
		t_env	*env = envm_init(env_arr);
		envm_set(&env, "NOVALUE", NULL);
		char	*args[] = {"env", NULL};
		char	*out = capture_stdout(builtin_env, args, &env);
		assert(strstr(out, "NOVALUE") == NULL);
		free(out);
		envm_free(&env);
	}
}

snow_main();
