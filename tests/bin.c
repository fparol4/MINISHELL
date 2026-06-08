/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.h"

#define TEST_SHARED_BUILTIN
#include "shared.c"

describe(bin_echo)
{
	char	*args[] = {"hello", "world", NULL};
	t_env	*env;
	char	*out;
	int		code;
	char	*args[] = {"-n", "hello", NULL};
	t_env	*env;
	char	*out;
	int		code;
	char	*args[] = {"-n", "-nn", "-nnn", "hello", NULL};
	t_env	*env;
	char	*out;
	int		code;

	it("prints arguments with newline")
	{
		env = NULL;
		out = capture_builtin(bin_echo, args, &env, &code);
		asserteq(code, 0);
		asserteq_str(out, "hello world\n");
		free(out);
	}
	it("supports -n")
	{
		env = NULL;
		out = capture_builtin(bin_echo, args, &env, &code);
		asserteq(code, 0);
		asserteq_str(out, "hello");
		free(out);
	}
	it("handles repeated -n flags")
	{
		env = NULL;
		out = capture_builtin(bin_echo, args, &env, &code);
		asserteq(code, 0);
		asserteq_str(out, "hello");
		free(out);
	}
}

describe(bin_export)
{
	char	*args[] = {"FOO=bar", NULL};
	t_env	*env;
	char	*args[] = {"KEY", NULL};
	t_env	*env;
	t_env	*node;
	char	*args[] = {"KEY=", NULL};
	t_env	*env;
	t_env	*node;
	char	*args[] = {"_A1=value", NULL};
	t_env	*env;
	char	*args[] = {"A+=value", NULL};
	t_env	*env;
	char	*args[] = {"1BAD=value", NULL};
	t_env	*env;
	char	*envp[] = {"Z=last", "A=first", NULL};
	char	*args[] = {NULL};
	t_env	*env;
	char	*out;
	int		code;

	it("sets a value")
	{
		env = NULL;
		asserteq(bin_export(args, &env), 0);
		asserteq_str(env_get(&env, "FOO"), "bar");
		env_free(&env);
	}
	it("creates a node without a value")
	{
		env = NULL;
		asserteq(bin_export(args, &env), 0);
		node = find_env_node(env, "KEY");
		assert(node != NULL);
		assert(node->value == NULL);
		env_free(&env);
	}
	it("creates a node with an empty value")
	{
		env = NULL;
		asserteq(bin_export(args, &env), 0);
		node = find_env_node(env, "KEY");
		assert(node != NULL);
		asserteq_str(node->value, "");
		env_free(&env);
	}
	it("accepts underscores and digits after the first char")
	{
		env = NULL;
		asserteq(bin_export(args, &env), 0);
		asserteq_str(env_get(&env, "_A1"), "value");
		env_free(&env);
	}
	it("rejects unsupported append assignment syntax")
	{
		env = NULL;
		asserteq(bin_export(args, &env), 1);
		assert(env_get(&env, "A") == NULL);
		env_free(&env);
	}
	it("rejects invalid identifiers")
	{
		env = NULL;
		asserteq(bin_export(args, &env), 1);
		env_free(&env);
	}
	it("prints sorted declare -x output when no args")
	{
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
	char	*envp[] = {"A=1", NULL};
	char	*args[] = {"A", NULL};
	t_env	*env;
	char	*envp[] = {"A=1", NULL};
	char	*args[] = {"MISSING", NULL};
	t_env	*env;

	it("removes exported values")
	{
		env = env_init(envp);
		asserteq(bin_unset(args, &env), 0);
		assert(env_get(&env, "A") == NULL);
		env_free(&env);
	}
	it("ignores missing keys")
	{
		env = env_init(envp);
		asserteq(bin_unset(args, &env), 0);
		asserteq_str(env_get(&env, "A"), "1");
		env_free(&env);
	}
}

describe(bin_env)
{
	char	*envp[] = {"A=1", "B", NULL};
	char	*args[] = {NULL};
	t_env	*env;
	char	*out;
	int		code;

	it("prints only entries with values")
	{
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
	char	*args[] = {NULL};
	t_env	*env;
	char	*out;
	int		code;

	it("returns success when cwd is available")
	{
		env = NULL;
		out = capture_builtin(bin_pwd, args, &env, &code);
		asserteq(code, 0);
		assert(out[0] == '/');
		free(out);
	}
}

describe(bin_exit)
{
	char	*args[] = {"1", "2", NULL};
	t_env	*env;
	char	*args[] = {"notanumber", NULL};
	t_env	*env;
	char	*args[] = {"42", NULL};
	t_env	*env;
	char	*args[] = {"9223372036854775808", NULL};
	t_env	*env;

	it("returns 1 for too many args without exiting")
	{
		env = NULL;
		asserteq(bin_exit(args, &env), 1);
		env_free(&env);
	}
	it("exits with 2 for non-numeric argument")
	{
		env = NULL;
		asserteq(fork_exit(args, &env), 2);
		env_free(&env);
	}
	it("exits with the given numeric code")
	{
		env = NULL;
		asserteq(fork_exit(args, &env), 42);
		env_free(&env);
	}
	it("exits with 2 for numeric overflow")
	{
		env = NULL;
		asserteq(fork_exit(args, &env), 2);
		env_free(&env);
	}
}

describe(bin_cd)
{
	char	cwd[4096];
	char	*envp[] = {"HOME=tests/_tmp", NULL};
	char	*args[] = {NULL};
	t_env	*env;
	char	*args[] = {NULL};
	t_env	*env;
	char	cwd[4096];
	char	*args[] = {"tests/_tmp", NULL};
	t_env	*env;
	char	cwd[4096];
	char	*args[] = {"tests", NULL};
	t_env	*env;
	char	*args[] = {"tests/_tmp/__minishell_cd_missing__", NULL};
	t_env	*env;
	char	cwd[4096];
	char	*args[] = {"-", NULL};
	char	*envp[] = {"OPWD=tests/_tmp", NULL};
	t_env	*env;
	char	*out;
	int		code;
	char	*args[] = {"-", NULL};
	t_env	*env;

	it("changes to HOME when no argument given")
	{
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
		env = NULL;
		asserteq(bin_cd(args, &env), 1);
		env_free(&env);
	}
	it("changes to a given path and updates PWD")
	{
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
		env = NULL;
		asserteq(bin_cd(args, &env), 1);
		env_free(&env);
	}
	it("cd - goes to OPWD and prints it")
	{
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
		env = NULL;
		asserteq(bin_cd(args, &env), 1);
		env_free(&env);
	}
}

snow_main();
