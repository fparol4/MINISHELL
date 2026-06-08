/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   core.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:38 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:38 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_core.h"

int	core_init(t_shell *shell, char **envp)
{
	if (!shell)
		return (1);
	shell->env = env_init(envp);
	if (!shell->env)
		return (sh_err(NULL, "environment initialization failed"), 1);
	if (env_set(&shell->env, ENV_ERRCODE, "0") == -1)
		return (env_free(&shell->env), sh_err(NULL,
				"status initialization failed"), 1);
	shell->running = TRUE;
	sh_sig_mode(SIG_INTERACTIVE);
	return (0);
}

void	core_destroy(t_shell *shell)
{
	if (!shell)
		return ;
	env_free(&shell->env);
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
