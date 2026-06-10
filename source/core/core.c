/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:34:14 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 16:34:14 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_core.h"
#include <readline/history.h>

int	core_init(t_shell *shell, char **envp)
{
	if (!shell)
		return (1);
	g_signal = 0;
	shell->env = env_init(envp);
	if (!shell->env)
	{
		sh_err(NULL, "environment initialization failed");
		return (1);
	}
	if (env_set(&shell->env, ENV_ERRCODE, "0") == -1)
	{
		env_free(&shell->env);
		sh_err(NULL, "status initialization failed");
		return (1);
	}
	shell->running = TRUE;
	sh_sig_mode(SIG_INTERACTIVE);
	return (0);
}

void	core_destroy(t_shell *shell)
{
	if (!shell)
		return ;
	env_free(&shell->env);
	rl_clear_history();
	shell->running = FALSE;
}

int	core_run(char **envp)
{
	t_shell	shell;
	int		status;

	if (core_init(&shell, envp))
		return (1);
	status = core_loop(&shell);
	core_destroy(&shell);
	return (status);
}
