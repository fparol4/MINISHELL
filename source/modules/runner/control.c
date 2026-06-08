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

static int	rn_cmd_redir_push(t_simple *simple, t_env **env, int saved[2])
{
	if (!simple->redirs.length)
		return (0);
	return (rn_redir_push((t_parser_redir *)simple->redirs.items,
			simple->redirs.length, env, saved));
}

static int	rn_cmd_run(char **args, t_env **env)
{
	int	status;

	if (!args[0])
		return (0);
	if (rn_exec_bin(args, env, &status))
		return (status);
	return (rn_exec_ext(args, env));
}

int	rn_exec_cmd(t_command *cmd, t_env **env)
{
	t_simple	*simple;
	char		**args;
	int			saved[2];
	int			status;

	if (!cmd)
		return (0);
	simple = &cmd->t_define.simple;
	args = rn_expand((char **)simple->args.items, env);
	if (!args)
		return (1);
	status = rn_cmd_redir_push(simple, env, saved);
	if (status)
		return (sh_freeargs(args), status);
	status = rn_cmd_run(args, env);
	sh_freeargs(args);
	if (simple->redirs.length && rn_redir_restore(saved))
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
