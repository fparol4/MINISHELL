/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/runner.h"

int	rn_exec_cmd(t_exnode *node, t_env **env)
{
	char	**args;
	int		saved[2];
	int		status;

	if (!node)
		return (0);
	args = rn_expand(node->args, env);
	if (!args)
		return (1);
	if (node->redir && rn_redir_push(node->redir, env, saved))
		return (sh_freeargs(args), 1);
	if (!args[0])
		status = 0;
	else if (!rn_exec_bin(args, env, &status))
		status = rn_exec_ext(args, env);
	sh_freeargs(args);
	if (node->redir && rn_redir_restore(saved))
		status = 1;
	return (status);
}

int	rn_exec_pipe(t_exnode *node, t_env **env)
{
	return (rn_pipe(node, env));
}

int	rn_execute(t_exnode *node, t_env **env)
{
	int	status;

	if (!node)
		return (0);
	if (node->type == CMD)
		status = rn_exec_cmd(node, env);
	else if (node->type == PIPE)
		status = rn_exec_pipe(node, env);
	else
		status = 1;
	rn_status_set(env, status);
	return (status);
}
