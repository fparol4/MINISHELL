/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tester.h"

describe(rn_expand)
{
	char	*raw[] = {"'$USER'", NULL};
	char	*envp[] = {"USER=fcardozo", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"\"hello $USER\"", NULL};
	char	*envp[] = {"USER=foo bar", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"$USER", NULL};
	char	*envp[] = {"USER=foo bar", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"status=$?", NULL};
	char	*envp[] = {"$?=42", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"pre$MISSING", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"\"\"", "''", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"$", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"$-bad", "$1", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"pre\"$USER\"post", NULL};
	char	*envp[] = {"USER=mid", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"$A-$B-$A", NULL};
	char	*envp[] = {"A=one", "B=two", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"keep", "$EMPTY", "$MISSING", "end", NULL};
	char	*envp[] = {"EMPTY=", NULL};
	t_env	*env;
	char	**out;
	char	*raw[] = {"$EMPTY\"\"", "\"\"$EMPTY", NULL};
	char	*envp[] = {"EMPTY=", NULL};
	t_env	*env;
	char	**out;

	it("keeps single quoted variables literal")
	{
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
