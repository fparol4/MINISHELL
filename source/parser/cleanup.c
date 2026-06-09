/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:09:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:09:16 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_parser.h"

static void	parser_cleanup(t_simple *simple);

void	parser_free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	parser_free_cmd(ast->root);
	free(ast);
}

void	parser_free_cmd(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == PNODE_PIPE)
	{
		parser_free_cmd(cmd->t_define.pipe.left);
		parser_free_cmd(cmd->t_define.pipe.right);
	}
	else if (cmd->type == PNODE_CMD)
		parser_cleanup(&cmd->t_define.simple);
	free(cmd);
}

void	parser_free_simple(t_simple *simple)
{
	if (!simple)
		return ;
	parser_cleanup(simple);
	free(simple);
}

static void	parser_cleanup(t_simple *simple)
{
	char			**args;
	t_parser_redir	*redirs;
	size_t			i;

	i = 0;
	args = (char **)simple->args.items;
	while (args && i < simple->args.length)
	{
		free(args[i]);
		i++;
	}
	ft_array_free(&simple->args);
	i = 0;
	redirs = (t_parser_redir *)simple->redirs.items;
	while (redirs && i < simple->redirs.length)
	{
		free(redirs[i].file);
		i++;
	}
	ft_array_free(&simple->redirs);
}
