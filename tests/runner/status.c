#include "../tester.h"

describe(rn_status)
{
	it("sets and gets the shell status")
	{
		t_env	*env;

		env = NULL;
		asserteq(rn_status_set(&env, 42), 0);
		asserteq(rn_status_get(&env), 42);
		env_free(&env);
	}
}
