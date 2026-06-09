/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:09:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:09:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

int	rn_redir_restore(int saved[2])
{
	if (saved[0] != -1 && dup2(saved[0], STDIN_FILENO) == -1)
		return (close(saved[0]), close(saved[1]), sh_err(NULL, "dup2 failed"),
			1);
	if (saved[1] != -1 && dup2(saved[1], STDOUT_FILENO) == -1)
		return (close(saved[0]), close(saved[1]), sh_err(NULL, "dup2 failed"),
			1);
	if (saved[0] != -1)
		close(saved[0]);
	if (saved[1] != -1)
		close(saved[1]);
	return (0);
}

static int	redir_save_stdio(int saved[2])
{
	saved[0] = -1;
	saved[1] = -1;
	saved[0] = dup(STDIN_FILENO);
	saved[1] = dup(STDOUT_FILENO);
	if (saved[0] == -1 || saved[1] == -1)
		return (rn_redir_restore(saved), sh_err(NULL, "dup failed"), 1);
	return (0);
}

int	rn_redir_push(t_parser_redir *redirs, size_t count, t_env **env,
		int saved[2])
{
	int	status;

	if (redir_save_stdio(saved))
		return (1);
	status = rn_redir_apply(redirs, count, env);
	if (status)
		return (rn_redir_restore(saved), status);
	return (0);
}
