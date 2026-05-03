#include <stdlib.h>
#include "../../src/types/minishell.h"

/* Mock: ls -l */
t_cmd	*mock_cmd_ls(void)
{
	t_cmd	*cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->type = CMD_COMMAND;
	cmd->args = malloc(sizeof(char *) * 3);
	cmd->args[0] = "ls";
	cmd->args[1] = "-l";
	cmd->args[2] = NULL;
	cmd->left = NULL;
	cmd->right = NULL;
	return (cmd);
}

/* Mock: ls -l | grep txt */
t_cmd	*mock_cmd_pipe(void)
{
	t_cmd	*pipe_node = malloc(sizeof(t_cmd));
	t_cmd	*left_cmd = malloc(sizeof(t_cmd));
	t_cmd	*right_cmd = malloc(sizeof(t_cmd));

	if (!pipe_node || !left_cmd || !right_cmd)
		return (NULL);

	pipe_node->type = CMD_PIPE;
	pipe_node->args = NULL;

	left_cmd->type = CMD_COMMAND;
	left_cmd->args = malloc(sizeof(char *) * 3);
	left_cmd->args[0] = "ls";
	left_cmd->args[1] = "-l";
	left_cmd->args[2] = NULL;
	left_cmd->left = NULL;
	left_cmd->right = NULL;

	right_cmd->type = CMD_COMMAND;
	right_cmd->args = malloc(sizeof(char *) * 3);
	right_cmd->args[0] = "grep";
	right_cmd->args[1] = "txt";
	right_cmd->args[2] = NULL;
	right_cmd->left = NULL;
	right_cmd->right = NULL;

	pipe_node->left = left_cmd;
	pipe_node->right = right_cmd;

	return (pipe_node);
}

/* Mock: make && ./minishell */
t_cmd	*mock_cmd_and(void)
{
	t_cmd	*and_node = malloc(sizeof(t_cmd));
	t_cmd	*left_cmd = malloc(sizeof(t_cmd));
	t_cmd	*right_cmd = malloc(sizeof(t_cmd));

	if (!and_node || !left_cmd || !right_cmd)
		return (NULL);

	and_node->type = CMD_AND;
	and_node->args = NULL;

	left_cmd->type = CMD_COMMAND;
	left_cmd->args = malloc(sizeof(char *) * 2);
	left_cmd->args[0] = "make";
	left_cmd->args[1] = NULL;
	left_cmd->left = NULL;
	left_cmd->right = NULL;

	right_cmd->type = CMD_COMMAND;
	right_cmd->args = malloc(sizeof(char *) * 2);
	right_cmd->args[0] = "./minishell";
	right_cmd->args[1] = NULL;
	right_cmd->left = NULL;
	right_cmd->right = NULL;

	and_node->left = left_cmd;
	and_node->right = right_cmd;

	return (and_node);
}
