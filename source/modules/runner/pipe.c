/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/runner.h"
#include <sys/wait.h>

typedef struct s_pipe_context
{
	t_exnode	*node;
	t_env		**env;
	int			in_fd;
	int			out_fd;
	int			pipefd[2];
}				t_pipe_context;

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

static void	rn_pipe_build(t_pipe_context *ctx, t_exnode *node, t_env **env,
		int in_fd, int out_fd)
{
	ctx->node = node;
	ctx->env = env;
	ctx->in_fd = in_fd;
	ctx->out_fd = out_fd;
}

static pid_t	rn_pipe_fork(t_pipe_context *ctx)
{
	pid_t	pid;
	int		in_dup;
	int		out_dup;

	pid = fork();
	if (pid == -1)
		return (sh_err(NULL, "fork failed"), -1);
	if (pid == 0)
	{
		in_dup = 0;
		out_dup = 0;
		if (ctx->in_fd != STDIN_FILENO)
			in_dup = dup2(ctx->in_fd, STDIN_FILENO);
		if (ctx->out_fd != STDOUT_FILENO)
			out_dup = dup2(ctx->out_fd, STDOUT_FILENO);
		if (in_dup == -1 || out_dup == -1)
		{
			rn_pipe_close(ctx->pipefd);
			sh_err(NULL, "dup2 failed");
			_exit(1);
		}
		rn_pipe_close(ctx->pipefd);
		signal(SIGINT, SIG_DFL);
		signal(SIGQUIT, SIG_DFL);
		_exit(rn_execute(ctx->node, ctx->env));
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
	t_pipe_context	left_ctx;
	t_pipe_context	right_ctx;
	pid_t			left_pid;
	pid_t			right_pid;

	if (!node || node->type != PIPE || !node->left || !node->right)
		return (sh_err(NULL, "invalid pipe node"), 1);
	left_ctx.pipefd[0] = -1;
	left_ctx.pipefd[1] = -1;
	if (pipe(left_ctx.pipefd) == -1)
		return (sh_err(NULL, "pipe failed"), 1);
	rn_pipe_build(&left_ctx, node->left, env, STDIN_FILENO, left_ctx.pipefd[1]);
	rn_pipe_build(&right_ctx, node->right, env, left_ctx.pipefd[0],
		STDOUT_FILENO);
	right_ctx.pipefd[0] = left_ctx.pipefd[0];
	right_ctx.pipefd[1] = left_ctx.pipefd[1];
	sh_sig_mode(SIG_EXEC);
	left_pid = rn_pipe_fork(&left_ctx);
	if (left_pid == -1)
		return (rn_pipe_close(left_ctx.pipefd), sh_sig_mode(SIG_INTERACTIVE), 1);
	right_pid = rn_pipe_fork(&right_ctx);
	rn_pipe_close(left_ctx.pipefd);
	if (right_pid == -1)
		return (rn_pipe_wait(left_pid), sh_sig_mode(SIG_INTERACTIVE), 1);
	rn_pipe_wait(left_pid);
	right_pid = rn_pipe_wait(right_pid);
	sh_sig_mode(SIG_INTERACTIVE);
	return (right_pid);
}
