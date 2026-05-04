# include "snow.h"
# include "envm.h"
# include "execm.h"
# include "libft.h"
# include <stdlib.h>
# include <string.h>

describe(execm_get_path)
{
	it("resolves system command to absolute path")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);
		char	*path = execm_get_path(env, "ls");
		assert(path != NULL);
		assert(strstr(path, "ls") != NULL);
		free(path);
		envm_free(&env);
	}

	it("returns NULL for non-existent command")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);
		char	*path = execm_get_path(env, "does_not_exist_xyz");
		assert(path == NULL);
		envm_free(&env);
	}

	it("returns local path when cmd contains a slash")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);
		char	*path = execm_get_path(env, "/bin/ls");
		assert(path != NULL);
		asserteq_str(path, "/bin/ls");
		free(path);
		envm_free(&env);
	}

	it("returns NULL for slash path that does not exist")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);
		char	*path = execm_get_path(env, "/no/such/binary_xyz");
		assert(path == NULL);
		envm_free(&env);
	}
}

describe(execm_is_builtin)
{
	it("returns 1 for cd")
	{
		asserteq(execm_is_builtin("cd"), 1);
	}

	it("returns 1 for all mandatory builtins")
	{
		asserteq(execm_is_builtin("echo"), 1);
		asserteq(execm_is_builtin("pwd"), 1);
		asserteq(execm_is_builtin("export"), 1);
		asserteq(execm_is_builtin("unset"), 1);
		asserteq(execm_is_builtin("env"), 1);
		asserteq(execm_is_builtin("exit"), 1);
	}

	it("returns 0 for external command grep")
	{
		asserteq(execm_is_builtin("grep"), 0);
	}

	it("returns 0 for empty string")
	{
		asserteq(execm_is_builtin(""), 0);
	}
}

snow_main();
