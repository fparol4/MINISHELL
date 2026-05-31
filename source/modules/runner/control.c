/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/03 00:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/runner.h"

int	rn_exec_cmd(t_command *cmd, t_env **env)
{
	t_simple	*simple;
	char		**args;
	int			saved[2];
	int			status;

	if (!cmd)
		return (0);
	simple = &cmd->t_define.simple;
	args = rn_expand(simple->args, env);
	if (!args)
		return (1);
	if (simple->redir_count
		&& rn_redir_push(simple->redirs, simple->redir_count, env, saved))
		return (sh_freeargs(args), 1);
	if (!args[0])
		status = 0;
	else if (!rn_exec_bin(args, env, &status))
		status = rn_exec_ext(args, env);
	sh_freeargs(args);
	if (simple->redir_count && rn_redir_restore(saved))
		status = 1;
	return (status);
}

int	rn_exec_pipe(t_command *cmd, t_env **env)
{
	return (rn_pipe(cmd, env));
}

int	rn_execute(t_command *cmd, t_env **env)
{
	int	status;

	if (!cmd)
		return (0);
	if (cmd->type == PNODE_CMD)
		status = rn_exec_cmd(cmd, env);
	else if (cmd->type == PNODE_PIPE)
		status = rn_exec_pipe(cmd, env);
	else
		status = 1;
	rn_status_set(env, status);
	return (status);
}
