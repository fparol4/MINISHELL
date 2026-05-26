#include "../../../headers/runner.h"
#include <sys/wait.h>

static void	rn_pipe_close(int pipefd[2])
{
	if (pipefd[0] != -1)
		close(pipefd[0]);
	if (pipefd[1] != -1)
		close(pipefd[1]);
}

static int	rn_pipe_status(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (1);
}

static pid_t	rn_pipe_fork(t_exnode *node, t_env **env, int in_fd, int out_fd,
		int pipefd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		return (sh_err(NULL, "fork failed"), -1);
	if (pid == 0)
	{
		if ((in_fd != STDIN_FILENO && dup2(in_fd, STDIN_FILENO) == -1)
			|| (out_fd != STDOUT_FILENO && dup2(out_fd, STDOUT_FILENO) == -1))
		{
			rn_pipe_close(pipefd);
			sh_err(NULL, "dup2 failed");
			_exit(1);
		}
		rn_pipe_close(pipefd);
		_exit(rn_execute(node, env));
	}
	return (pid);
}

static int	rn_pipe_wait(pid_t pid)
{
	int	status;

	if (waitpid(pid, &status, 0) == -1)
		return (1);
	return (rn_pipe_status(status));
}

int	rn_pipe(t_exnode *node, t_env **env)
{
	int		pipefd[2];
	pid_t	left_pid;
	pid_t	right_pid;

	if (!node || node->type != PIPE || !node->left || !node->right)
		return (sh_err(NULL, "invalid pipe node"), 1);
	pipefd[0] = -1;
	pipefd[1] = -1;
	if (pipe(pipefd) == -1)
		return (sh_err(NULL, "pipe failed"), 1);
	left_pid = rn_pipe_fork(node->left, env, STDIN_FILENO, pipefd[1], pipefd);
	if (left_pid == -1)
		return (rn_pipe_close(pipefd), 1);
	right_pid = rn_pipe_fork(node->right, env, pipefd[0], STDOUT_FILENO,
			pipefd);
	rn_pipe_close(pipefd);
	if (right_pid == -1)
		return (rn_pipe_wait(left_pid), 1);
	rn_pipe_wait(left_pid);
	return (rn_pipe_wait(right_pid));
}
