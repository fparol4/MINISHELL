/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   run.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 00:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 00:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/core.h"

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
