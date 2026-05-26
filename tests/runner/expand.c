#include "../tester.h"

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
