# include "redir.h"
# include "../pipeline/pipeline.h"
# include "../../lib/libft/libft.h"
# include <fcntl.h>
# include <stdio.h>
# include <unistd.h>

static int	run_with_fd(t_cmd *cmd_node, t_env **env_list, int fd, int std_fd)
{
	int	saved_fd;
	int	status;

	saved_fd = dup(std_fd);
	if (saved_fd == -1)
	{
		close(fd);
		return (1);
	}
	if (dup2(fd, std_fd) == -1)
	{
		close(fd);
		close(saved_fd);
		return (1);
	}
	close(fd);
	status = pipeline_exec_node(cmd_node->left, env_list);
	if (dup2(saved_fd, std_fd) == -1)
	{
		close(saved_fd);
		return (1);
	}
	close(saved_fd);
	return (status);
}

static int	open_target(t_cmd *cmd_node, int flags)
{
	int	fd;

	if (!cmd_node || !cmd_node->args || !cmd_node->args[0])
		return (-1);
	fd = open(cmd_node->args[0], flags, 0644);
	if (fd == -1)
		perror(cmd_node->args[0]);
	return (fd);
}

int	redir_exec_out(t_cmd *cmd_node, t_env **env_list)
{
	int	fd;

	fd = open_target(cmd_node, O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1)
		return (1);
	return (run_with_fd(cmd_node, env_list, fd, STDOUT_FILENO));
}

int	redir_exec_append(t_cmd *cmd_node, t_env **env_list)
{
	int	fd;

	fd = open_target(cmd_node, O_WRONLY | O_CREAT | O_APPEND);
	if (fd == -1)
		return (1);
	return (run_with_fd(cmd_node, env_list, fd, STDOUT_FILENO));
}

int	redir_exec_in(t_cmd *cmd_node, t_env **env_list)
{
	int	fd;

	if (!cmd_node || !cmd_node->args || !cmd_node->args[0])
		return (1);
	fd = open(cmd_node->args[0], O_RDONLY);
	if (fd == -1)
	{
		perror(cmd_node->args[0]);
		return (1);
	}
	return (run_with_fd(cmd_node, env_list, fd, STDIN_FILENO));
}
