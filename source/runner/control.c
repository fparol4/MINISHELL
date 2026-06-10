/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   control.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:03 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 17:14:03 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_runner.h"
#include "redir/_redir.h"

static int	rn_cmd_run(char **args, t_env **env)
{
	int	status;

	if (!args[0])
		return (0);
	if (rn_exec_bin(args, env, &status))
		return (status);
	status = rn_exec_ext(args, env);
	return (status);
}

static int	rn_exec_redirs(t_simple *simple, t_env **env, int heredoc_fd,
		int saved[2])
{
	t_redir_ctx	redir;

	if (!simple->redirs.length)
		return (0);
	redir.redirs = (t_parser_redir *)simple->redirs.items;
	redir.count = simple->redirs.length;
	redir.env = env;
	redir.input_fd = heredoc_fd;
	return (rn_redir_push(&redir, saved));
}

static int	rn_exec_cmd(t_command *cmd, t_env **env, int heredoc_fd)
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
	status = rn_exec_redirs(simple, env, heredoc_fd, saved);
	if (status)
	{
		sh_freeargs(args);
		return (status);
	}
	status = rn_cmd_run(args, env);
	sh_freeargs(args);
	if (simple->redirs.length && rn_redir_restore(saved))
		status = 1;
	return (status);
}

int	rn_execute(t_command *cmd, t_env **env, int heredoc_fd, t_ast *ast)
{
	int	status;

	if (!cmd)
		return (0);
	if (cmd->type == PNODE_CMD)
		status = rn_exec_cmd(cmd, env, heredoc_fd);
	else if (cmd->type == PNODE_PIPE)
		status = rn_pipe(cmd, env, ast);
	else
		status = 1;
	rn_status_set(env, status);
	return (status);
}
