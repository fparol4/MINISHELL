/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tester.h"

describe(rn_pipe)
{
	{
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"echo", "hello", NULL};
	char			*right_args[] = {"wc", "-c", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	char			*out;
	int				status;

	it("pipes left stdout into right stdin")
	{
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "6\n");
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		free(out);
		env_free(&env);
	}
	}
	{
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"not_a_real_command_xyz", NULL};
	char			*right_args[] = {"true", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	int				status;

	it("returns the right-side status")
	{
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		status = rn_execute(&node, &env, STDIN_FILENO);
		asserteq(status, 0);
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		env_free(&env);
	}
	}
	{
	char			path[] = "tests/_tmp/minishell_pipe_in_XXXXXX";
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"cat", NULL};
	char			*right_args[] = {"wc", "-c", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redir;
	char			*out;
	int				fd;
	int				status;

	it("reads left input redirection before piping")
	{
		fd = rn_test_temp(path);
		write(fd, "hello\n", 6);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_redir(&redir, REDIR_IN, path, 1);
		rn_test_attach_redirs(&left, &redir, 1);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "6\n");
		unlink(path);
		free(out);
		env_free(&env);
	}
	}
	{
	char			path[] = "tests/_tmp/minishell_pipe_out_XXXXXX";
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"echo", "hi", NULL};
	char			*right_args[] = {"cat", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redir;
	char			*out;
	int				fd;
	int				status;

	it("writes right output redirection after reading from the pipe")
	{
		fd = rn_test_temp(path);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_redir(&redir, REDIR_OUT, path, 1);
		rn_test_attach_redirs(&right, &redir, 1);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "");
		free(out);
		out = rn_test_readfile(path);
		assert(out != NULL);
		asserteq_str(out, "hi\n");
		unlink(path);
		free(out);
		env_free(&env);
	}
	}
	{
	char			path[] = "tests/_tmp/minishell_pipe_left_out_XXXXXX";
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"echo", "hi", NULL};
	char			*right_args[] = {"wc", "-c", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redir;
	char			*out;
	int				fd;
	int				status;

	it("lets left output redirection override the pipe stdout")
	{
		fd = rn_test_temp(path);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_redir(&redir, REDIR_OUT, path, 1);
		rn_test_attach_redirs(&left, &redir, 1);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "0\n");
		free(out);
		out = rn_test_readfile(path);
		assert(out != NULL);
		asserteq_str(out, "hi\n");
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		unlink(path);
		free(out);
		env_free(&env);
	}
	}
	{
	char			path[] = "tests/_tmp/minishell_pipe_right_in_XXXXXX";
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"echo", "pipe", NULL};
	char			*right_args[] = {"cat", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redir;
	char			*out;
	int				fd;
	int				status;

	it("lets right input redirection override the pipe stdin")
	{
		fd = rn_test_temp(path);
		write(fd, "file\n", 5);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_redir(&redir, REDIR_IN, path, 1);
		rn_test_attach_redirs(&right, &redir, 1);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "file\n");
		unlink(path);
		free(out);
		env_free(&env);
	}
	}
	{
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"cat", NULL};
	char			*right_args[] = {"wc", "-c", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redir;
	char			*out;
	int				status;

	it("pipes heredoc input through the left command")
	{
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_redir(&redir, REDIR_HEREDOC, "EOF", 1);
		rn_test_attach_redirs(&left, &redir, 1);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		asserteq(rn_test_with_stdin("abc\nEOF\n", &node, &env, &status, &out),
			0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "4\n");
		free(out);
		env_free(&env);
	}
	}
	{
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"echo", "pipe", NULL};
	char			*right_args[] = {"cat", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redir;
	char			*out;
	int				status;

	it("reads a right-side heredoc from the shell instead of the pipe")
	{
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_redir(&redir, REDIR_HEREDOC, "EOF", 1);
		rn_test_attach_redirs(&right, &redir, 1);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		asserteq(rn_test_with_stdin("heredoc\nEOF\n", &node, &env, &status,
				&out), 0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "heredoc\n");
		free(out);
		env_free(&env);
	}
	}
	{
	char			path_a[] = "tests/_tmp/minishell_pipe_in_a_XXXXXX";
	char			path_b[] = "tests/_tmp/minishell_pipe_in_b_XXXXXX";
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"cat", NULL};
	char			*right_args[] = {"cat", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redirs[2];
	char			*out;
	int				fd;
	int				status;

	it("keeps the last input redirection inside a piped command")
	{
		fd = rn_test_temp(path_a);
		write(fd, "first\n", 6);
		close(fd);
		fd = rn_test_temp(path_b);
		write(fd, "second\n", 7);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_redir(&redirs[0], REDIR_IN, path_a, 1);
		rn_test_redir(&redirs[1], REDIR_IN, path_b, 1);
		rn_test_attach_redirs(&left, redirs, 2);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "second\n");
		unlink(path_a);
		unlink(path_b);
		free(out);
		env_free(&env);
	}
	}
	{
	char			path_a[] = "tests/_tmp/minishell_pipe_out_a_XXXXXX";
	char			path_b[] = "tests/_tmp/minishell_pipe_out_b_XXXXXX";
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"echo", "pipe", NULL};
	char			*right_args[] = {"cat", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redirs[2];
	char			*out;
	int				fd;
	int				status;

	it("keeps the last output redirection inside a piped command")
	{
		fd = rn_test_temp(path_a);
		close(fd);
		fd = rn_test_temp(path_b);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_redir(&redirs[0], REDIR_OUT, path_a, 1);
		rn_test_redir(&redirs[1], REDIR_OUT, path_b, 1);
		rn_test_attach_redirs(&right, redirs, 2);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "");
		free(out);
		out = rn_test_readfile(path_a);
		assert(out != NULL);
		asserteq_str(out, "");
		free(out);
		out = rn_test_readfile(path_b);
		assert(out != NULL);
		asserteq_str(out, "pipe\n");
		unlink(path_a);
		unlink(path_b);
		free(out);
		env_free(&env);
	}
	}
	{
	char			path[] = "tests/_tmp/minishell_pipe_hd_override_XXXXXX";
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"cat", NULL};
	char			*right_args[] = {"cat", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redirs[2];
	char			*out;
	int				fd;
	int				status;

	it("lets a later file input override a heredoc inside a pipe")
	{
		fd = rn_test_temp(path);
		write(fd, "file\n", 5);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_redir(&redirs[0], REDIR_HEREDOC, "EOF", 1);
		rn_test_redir(&redirs[1], REDIR_IN, path, 1);
		rn_test_attach_redirs(&left, redirs, 2);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		asserteq(rn_test_with_stdin("heredoc\nEOF\n", &node, &env, &status,
				&out), 0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "file\n");
		unlink(path);
		free(out);
		env_free(&env);
	}
	}
	{
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*inner_left_args[] = {"echo", "hi", NULL};
	char			*inner_right_args[] = {"cat", NULL};
	char			*outer_right_args[] = {"wc", "-c", NULL};
	t_env			*env;
	t_command		inner_left;
	t_command		inner_right;
	t_command		inner;
	t_command		outer_right;
	t_command		outer;
	char			*out;
	int				status;

	it("executes nested pipe nodes")
	{
		env = env_init(envp);
		rn_test_node(&inner_left, PNODE_CMD, inner_left_args, NULL, NULL);
		rn_test_node(&inner_right, PNODE_CMD, inner_right_args, NULL, NULL);
		rn_test_node(&inner, PNODE_PIPE, NULL, &inner_left, &inner_right);
		rn_test_node(&outer_right, PNODE_CMD, outer_right_args, NULL, NULL);
		rn_test_node(&outer, PNODE_PIPE, NULL, &inner, &outer_right);
		out = rn_test_capture_execute(&outer, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "3\n");
		free(out);
		env_free(&env);
	}
	}
	{
	char			path[] = "tests/_tmp/minishell_pipe_missing_input_XXXXXX";
	char			*envp[] = {"PATH=/usr/bin:/bin", NULL};
	char			*left_args[] = {"echo", "hi", NULL};
	char			*right_args[] = {"cat", NULL};
	t_env			*env;
	t_command		left;
	t_command		right;
	t_command		node;
	t_parser_redir	redir;
	int				fd;
	int				status;

	it("returns a failing branch status when pipe redirection setup fails")
	{
		fd = rn_test_temp(path);
		close(fd);
		unlink(path);
		env = env_init(envp);
		rn_test_node(&left, PNODE_CMD, left_args, NULL, NULL);
		rn_test_node(&right, PNODE_CMD, right_args, NULL, NULL);
		rn_test_redir(&redir, REDIR_IN, path, 1);
		rn_test_attach_redirs(&right, &redir, 1);
		rn_test_node(&node, PNODE_PIPE, NULL, &left, &right);
		status = rn_execute(&node, &env, STDIN_FILENO);
		asserteq(status, 1);
		asserteq_str(env_get(&env, ENV_ERRCODE), "1");
		env_free(&env);
	}
	}
}
