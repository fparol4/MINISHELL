#include "tester.h"
#include "../headers/core.h"

int	core_loop(t_shell *shell)
{
	(void)shell;
	return (0);
}

describe(core_init)
{
	it("initializes shell env and status")
	{
		char	*envp[] = {"PWD=/tmp", "SHLVL=1", NULL};
		t_shell	shell;

		shell.env = NULL;
		shell.running = FALSE;
		asserteq(core_init(&shell, envp), 0);
		assert(shell.env != NULL);
		assert(shell.running == TRUE);
		asserteq_str(env_get(&shell.env, ENV_ERRCODE), "0");
		core_destroy(&shell);
	}

	it("initializes with a minimal env when envp is NULL")
	{
		t_shell	shell;

		shell.env = NULL;
		shell.running = FALSE;
		asserteq(core_init(&shell, NULL), 0);
		assert(shell.env != NULL);
		assert(shell.running == TRUE);
		assert(env_get(&shell.env, ENV_PWD) != NULL);
		asserteq_str(env_get(&shell.env, ENV_ERRCODE), "0");
		core_destroy(&shell);
	}
}

describe(core_destroy)
{
	it("frees env and marks shell as stopped")
	{
		char	*envp[] = {"PWD=/tmp", NULL};
		t_shell	shell;

		shell.env = NULL;
		shell.running = FALSE;
		asserteq(core_init(&shell, envp), 0);
		core_destroy(&shell);
		assert(shell.env == NULL);
		assert(shell.running == FALSE);
	}
}

snow_main();
