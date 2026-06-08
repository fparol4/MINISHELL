#include "tester.h"

describe(env_init)
{
	it("parses entries into a linked list")
	{
		char	*envp[] = {"USER=fcardozo", "HOME=tests/_tmp", NULL};
		t_env	*env;

		env = env_init(envp);
		assert(env != NULL);
		asserteq_str(env_get(&env, "USER"), "fcardozo");
		asserteq_str(env_get(&env, "HOME"), "tests/_tmp");
		env_free(&env);
	}

	it("keeps entries without value")
	{
		char	*envp[] = {"EMPTY", NULL};
		t_env	*env;

		env = env_init(envp);
		assert(env != NULL);
		assert(env_get(&env, "EMPTY") == NULL);
		env_free(&env);
	}
}

describe(env_get)
{
	it("returns NULL for missing keys")
	{
		char	*envp[] = {"A=1", NULL};
		t_env	*env;

		env = env_init(envp);
		assert(env_get(&env, "B") == NULL);
		env_free(&env);
	}
}

describe(env_size)
{
	it("counts nodes")
	{
		char	*envp[] = {"A=1", "B=2", NULL};
		t_env	*env;

		env = env_init(envp);
		asserteq(env_size(&env), 2);
		env_free(&env);
	}
}

describe(env_set)
{
	it("adds a new key")
	{
		t_env	*env;

		env = NULL;
		asserteq(env_set(&env, "A", "1"), 0);
		asserteq_str(env_get(&env, "A"), "1");
		env_free(&env);
	}

	it("updates an existing key")
	{
		char	*envp[] = {"A=1", NULL};
		t_env	*env;

		env = env_init(envp);
		asserteq(env_set(&env, "A", "2"), 0);
		asserteq_str(env_get(&env, "A"), "2");
		env_free(&env);
	}
}

describe(env_unset)
{
	it("removes a key")
	{
		char	*envp[] = {"A=1", "B=2", NULL};
		t_env	*env;

		env = env_init(envp);
		asserteq(env_unset(&env, "A"), 0);
		assert(env_get(&env, "A") == NULL);
		asserteq_str(env_get(&env, "B"), "2");
		env_free(&env);
	}
}

describe(env_toarr)
{
	it("creates KEY=VALUE strings")
	{
		char	*envp[] = {"A=1", "B=2", NULL};
		t_env	*env;
		char	**arr;

		env = env_init(envp);
		arr = env_toarr(&env);
		assert(arr != NULL);
		asserteq_str(arr[0], "A=1");
		asserteq_str(arr[1], "B=2");
		assert(arr[2] == NULL);
		sh_freeargs(arr);
		env_free(&env);
	}
}

snow_main();
