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

	it("expands status from ENV_ERRCODE")
	{
		char	*raw[] = {"status=$?", NULL};
		char	*envp[] = {"$?=42", NULL};
		t_env	*env;
		char	**out;

		env = env_init(envp);
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "status=42");
		assert(out[1] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}

	it("expands unknown variables to an empty string")
	{
		char	*raw[] = {"pre$MISSING", NULL};
		t_env	*env;
		char	**out;

		env = NULL;
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "pre");
		assert(out[1] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}

	it("preserves empty quoted arguments")
	{
		char	*raw[] = {"\"\"", "''", NULL};
		t_env	*env;
		char	**out;

		env = NULL;
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "");
		asserteq_str(out[1], "");
		assert(out[2] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}

	it("keeps a lone dollar literal")
	{
		char	*raw[] = {"$", NULL};
		t_env	*env;
		char	**out;

		env = NULL;
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "$");
		assert(out[1] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}

	it("keeps dollar literal before an invalid variable start")
	{
		char	*raw[] = {"$-bad", "$1", NULL};
		t_env	*env;
		char	**out;

		env = NULL;
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "$-bad");
		asserteq_str(out[1], "$1");
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

	it("expands multiple variables in one word in order")
	{
		char	*raw[] = {"$A-$B-$A", NULL};
		char	*envp[] = {"A=one", "B=two", NULL};
		t_env	*env;
		char	**out;

		env = env_init(envp);
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "one-two-one");
		assert(out[1] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}

	it("removes unquoted empty expansions when they are the whole arg")
	{
		char	*raw[] = {"keep", "$EMPTY", "$MISSING", "end", NULL};
		char	*envp[] = {"EMPTY=", NULL};
		t_env	*env;
		char	**out;

		env = env_init(envp);
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "keep");
		asserteq_str(out[1], "end");
		assert(out[2] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}

	it("keeps word count for adjacent quoted and unquoted empty parts")
	{
		char	*raw[] = {"$EMPTY\"\"", "\"\"$EMPTY", NULL};
		char	*envp[] = {"EMPTY=", NULL};
		t_env	*env;
		char	**out;

		env = env_init(envp);
		out = rn_expand(raw, &env);
		assert(out != NULL);
		asserteq_str(out[0], "");
		asserteq_str(out[1], "");
		assert(out[2] == NULL);
		sh_freeargs(out);
		env_free(&env);
	}
}
