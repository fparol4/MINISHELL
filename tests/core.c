/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/minishell.h"
#include "tester.h"

int	core_loop(t_shell *shell)
{
	(void)shell;
	return (0);
}

describe(core_init)
{
	char	*envp[] = {"PWD=tests/_tmp", "SHLVL=1", NULL};
	t_shell	shell;
	t_shell	shell;

	it("initializes shell env and status")
	{
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
	char	*envp[] = {"PWD=tests/_tmp", NULL};
	t_shell	shell;

	it("frees env and marks shell as stopped")
	{
		shell.env = NULL;
		shell.running = FALSE;
		asserteq(core_init(&shell, envp), 0);
		core_destroy(&shell);
		assert(shell.env == NULL);
		assert(shell.running == FALSE);
	}
}

snow_main();
