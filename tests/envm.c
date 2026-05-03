# include "snow.h"
# include "env.h"
# include "libft.h"
# include <stdlib.h>
# include <string.h>

describe(envm_init)
{
	it("parses envp into a linked list")
	{
		char	*mock_envp[] = {"USER=tester", "HOME=/home/tester", NULL};
		t_env	*env;

		env = envm_init(mock_envp);
		assert(env != NULL);
		assert(strcmp(envm_get(env, "USER"), "tester") == 0);
		assert(strcmp(envm_get(env, "HOME"), "/home/tester") == 0);
		envm_free(&env);
	}

	it("increments SHLVL by 1")
	{
		char	*mock_envp[] = {"SHLVL=2", NULL};
		t_env	*env;

		env = envm_init(mock_envp);
		assert(env != NULL);
		assert(strcmp(envm_get(env, "SHLVL"), "3") == 0);
		envm_free(&env);
	}

	it("creates minimal env when envp is NULL")
	{
		t_env	*env;

		env = envm_init(NULL);
		assert(env != NULL);
		assert(envm_get(env, "PWD") != NULL);
		assert(envm_get(env, "SHLVL") != NULL);
		assert(strcmp(envm_get(env, "SHLVL"), "1") == 0);
		envm_free(&env);
	}

	it("handles entry with no equals sign (no value)")
	{
		char	*mock_envp[] = {"VARNOVALUE", NULL};
		t_env	*env;

		env = envm_init(mock_envp);
		assert(env != NULL);
		assert(envm_get(env, "VARNOVALUE") == NULL);
		envm_free(&env);
	}
}

describe(envm_get)
{
	it("returns NULL for missing key")
	{
		char	*mock_envp[] = {"USER=tester", NULL};
		t_env	*env;

		env = envm_init(mock_envp);
		assert(envm_get(env, "MISSING") == NULL);
		envm_free(&env);
	}

	it("returns NULL on empty list")
	{
		assert(envm_get(NULL, "KEY") == NULL);
	}
}

describe(envm_set)
{
	it("inserts a new key")
	{
		t_env	*env;

		env = NULL;
		envm_set(&env, "MYVAR", "hello");
		assert(strcmp(envm_get(env, "MYVAR"), "hello") == 0);
		envm_free(&env);
	}

	it("updates an existing key")
	{
		t_env	*env;

		env = NULL;
		envm_set(&env, "USER", "old");
		envm_set(&env, "USER", "new_user");
		assert(strcmp(envm_get(env, "USER"), "new_user") == 0);
		envm_free(&env);
	}

	it("sets a key with NULL value")
	{
		t_env	*env;

		env = NULL;
		envm_set(&env, "NOVALUE", NULL);
		assert(envm_get(env, "NOVALUE") == NULL);
		envm_free(&env);
	}
}

describe(envm_unset)
{
	it("removes a key from the list")
	{
		char	*mock_envp[] = {"USER=tester", "HOME=/home/tester", NULL};
		t_env	*env;

		env = envm_init(mock_envp);
		envm_unset(&env, "USER");
		assert(envm_get(env, "USER") == NULL);
		assert(strcmp(envm_get(env, "HOME"), "/home/tester") == 0);
		envm_free(&env);
	}

	it("handles removal of non-existent key safely")
	{
		char	*mock_envp[] = {"USER=tester", NULL};
		t_env	*env;

		env = envm_init(mock_envp);
		envm_unset(&env, "GHOST");
		assert(strcmp(envm_get(env, "USER"), "tester") == 0);
		envm_free(&env);
	}

	it("removes head node correctly")
	{
		char	*mock_envp[] = {"A=1", "B=2", NULL};
		t_env	*env;

		env = envm_init(mock_envp);
		envm_unset(&env, "A");
		assert(envm_get(env, "A") == NULL);
		assert(strcmp(envm_get(env, "B"), "2") == 0);
		envm_free(&env);
	}
}

describe(envm_toarr)
{
	it("produces KEY=VALUE formatted strings")
	{
		t_env	*env;
		char	**arr;

		env = NULL;
		envm_set(&env, "FOO", "bar");
		envm_set(&env, "BAZ", "qux");
		arr = envm_toarr(env);
		assert(arr != NULL);
		assert(strcmp(arr[0], "FOO=bar") == 0);
		assert(strcmp(arr[1], "BAZ=qux") == 0);
		assert(arr[2] == NULL);
		free(arr[0]);
		free(arr[1]);
		free(arr);
		envm_free(&env);
	}

	it("emits KEY= for NULL value entries")
	{
		t_env	*env;
		char	**arr;

		env = NULL;
		envm_set(&env, "EMPTY", NULL);
		arr = envm_toarr(env);
		assert(arr != NULL);
		assert(strcmp(arr[0], "EMPTY=") == 0);
		assert(arr[1] == NULL);
		free(arr[0]);
		free(arr);
		envm_free(&env);
	}
}

snow_main();
