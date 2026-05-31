#include "../../headers/parser_internal.h"

static void	free_simple_data(t_simple *simple);

void	parser_free_ast(t_ast *ast)
{
	if (!ast)
		return ;
	parser_free_command(ast->root);
	free(ast);
}

void	parser_free_command(t_command *cmd)
{
	if (!cmd)
		return ;
	if (cmd->type == PNODE_PIPE)
	{
		parser_free_command(cmd->t_define.pipe.left);
		parser_free_command(cmd->t_define.pipe.right);
	}
	else if (cmd->type == PNODE_CMD)
		free_simple_data(&cmd->t_define.simple);
	free(cmd);
}

void	parser_free_simple(t_simple *simple)
{
	if (!simple)
		return ;
	free_simple_data(simple);
	free(simple);
}

static void	free_simple_data(t_simple *simple)
{
	unsigned int	i;

	i = 0;
	while (simple->args && simple->args[i])
	{
		free(simple->args[i]);
		i++;
	}
	free(simple->args);
	i = 0;
	while (i < simple->redir_count)
	{
		free(simple->redirs[i].file);
		i++;
	}
	free(simple->redirs);
}
