# include "pipeline.h"
# include "../execm/execm.h"
# include "../redir/redir.h"
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

static int	wait_status(pid_t pid)
{
	int	status;

	status = 0;
	if (waitpid(pid, &status, 0) == -1)
		return (1);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static int	exec_logical(t_cmd *cmd_node, t_env **env_list)
{
	int	status;

	status = pipeline_exec_node(cmd_node->left, env_list);
	if (cmd_node->type == CMD_AND)
	{
		if (status == 0)
			return (pipeline_exec_node(cmd_node->right, env_list));
		return (status);
	}
	if (status != 0)
		return (pipeline_exec_node(cmd_node->right, env_list));
	return (status);
}

int	pipeline_exec_node(t_cmd *cmd_node, t_env **env_list)
{
	if (!cmd_node)
		return (0);
	if (cmd_node->type == CMD_COMMAND)
		return (execm_exec_simple(cmd_node, env_list));
	if (cmd_node->type == CMD_PIPE)
		return (pipeline_exec_pipe(cmd_node, env_list));
	if (cmd_node->type == CMD_REDIR_IN)
		return (redir_exec_in(cmd_node, env_list));
	if (cmd_node->type == CMD_REDIR_OUT)
		return (redir_exec_out(cmd_node, env_list));
	if (cmd_node->type == CMD_REDIR_APPEND)
		return (redir_exec_append(cmd_node, env_list));
	if (cmd_node->type == CMD_AND || cmd_node->type == CMD_OR)
		return (exec_logical(cmd_node, env_list));
	return (1);
}

int	pipeline_exec_pipe(t_cmd *cmd_node, t_env **env_list)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;
	int		right_status;

	if (!cmd_node || !cmd_node->left || !cmd_node->right || pipe(pipefd) == -1)
		return (1);
	left_pid = fork();
	if (left_pid == -1)
		return (close(pipefd[0]), close(pipefd[1]), 1);
	if (left_pid == 0)
	{
		dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(pipeline_exec_node(cmd_node->left, env_list));
	}
	right_pid = fork();
	if (right_pid == -1)
	{
		close(pipefd[0]);
		close(pipefd[1]);
		wait_status(left_pid);
		return (1);
	}
	if (right_pid == 0)
	{
		dup2(pipefd[0], STDIN_FILENO);
		close(pipefd[0]);
		close(pipefd[1]);
		exit(pipeline_exec_node(cmd_node->right, env_list));
	}
	close(pipefd[0]);
	close(pipefd[1]);
	wait_status(left_pid);
	right_status = wait_status(right_pid);
	return (right_status);
}
