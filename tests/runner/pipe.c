#include "../tester.h"

describe(rn_pipe)
{
	it("pipes left stdout into right stdin")
	{
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"echo", "hello", NULL};
		char	*right_args[] = {"wc", "-c", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		char	*out;
		int		status;

		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_node(&node, PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "6\n");
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		free(out);
		env_free(&env);
	}

	it("returns the right-side status")
	{
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"not_a_real_command_xyz", NULL};
		char	*right_args[] = {"true", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		int		status;

		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_node(&node, PIPE, NULL, &left, &right);
		status = rn_execute(&node, &env);
		asserteq(status, 0);
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		env_free(&env);
	}

	it("reads left input redirection before piping")
	{
		char	path[] = "/tmp/minishell_pipe_in_XXXXXX";
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"cat", NULL};
		char	*right_args[] = {"wc", "-c", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		t_redir	redir;
		char	*out;
		int		fd;
		int		status;

		fd = rn_test_temp(path);
		write(fd, "hello\n", 6);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_redir(&redir, IN_F, path, 1);
		left.redir = &redir;
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_node(&node, PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "6\n");
		unlink(path);
		free(out);
		env_free(&env);
	}

	it("writes right output redirection after reading from the pipe")
	{
		char	path[] = "/tmp/minishell_pipe_out_XXXXXX";
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"echo", "hi", NULL};
		char	*right_args[] = {"cat", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		t_redir	redir;
		char	*out;
		int		fd;
		int		status;

		fd = rn_test_temp(path);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_redir(&redir, OUT_T, path, 1);
		right.redir = &redir;
		rn_test_node(&node, PIPE, NULL, &left, &right);
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

	it("lets left output redirection override the pipe stdout")
	{
		char	path[] = "/tmp/minishell_pipe_left_out_XXXXXX";
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"echo", "hi", NULL};
		char	*right_args[] = {"wc", "-c", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		t_redir	redir;
		char	*out;
		int		fd;
		int		status;

		fd = rn_test_temp(path);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_redir(&redir, OUT_T, path, 1);
		left.redir = &redir;
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_node(&node, PIPE, NULL, &left, &right);
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

	it("lets right input redirection override the pipe stdin")
	{
		char	path[] = "/tmp/minishell_pipe_right_in_XXXXXX";
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"echo", "pipe", NULL};
		char	*right_args[] = {"cat", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		t_redir	redir;
		char	*out;
		int		fd;
		int		status;

		fd = rn_test_temp(path);
		write(fd, "file\n", 5);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_redir(&redir, IN_F, path, 1);
		right.redir = &redir;
		rn_test_node(&node, PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "file\n");
		unlink(path);
		free(out);
		env_free(&env);
	}

	it("pipes heredoc input through the left command")
	{
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"cat", NULL};
		char	*right_args[] = {"wc", "-c", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		t_redir	redir;
		char	*out;
		int		status;

		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_redir(&redir, IN_H, "EOF", 1);
		left.redir = &redir;
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_node(&node, PIPE, NULL, &left, &right);
		asserteq(rn_test_with_stdin("abc\nEOF\n", &node, &env, &status, &out), 0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "4\n");
		free(out);
		env_free(&env);
	}

	it("keeps the last input redirection inside a piped command")
	{
		char	path_a[] = "/tmp/minishell_pipe_in_a_XXXXXX";
		char	path_b[] = "/tmp/minishell_pipe_in_b_XXXXXX";
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"cat", NULL};
		char	*right_args[] = {"cat", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		t_redir	first;
		t_redir	second;
		char	*out;
		int		fd;
		int		status;

		fd = rn_test_temp(path_a);
		write(fd, "first\n", 6);
		close(fd);
		fd = rn_test_temp(path_b);
		write(fd, "second\n", 7);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_redir(&first, IN_F, path_a, 1);
		rn_test_redir(&second, IN_F, path_b, 1);
		first.next = &second;
		left.redir = &first;
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_node(&node, PIPE, NULL, &left, &right);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "second\n");
		unlink(path_a);
		unlink(path_b);
		free(out);
		env_free(&env);
	}

	it("keeps the last output redirection inside a piped command")
	{
		char	path_a[] = "/tmp/minishell_pipe_out_a_XXXXXX";
		char	path_b[] = "/tmp/minishell_pipe_out_b_XXXXXX";
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"echo", "pipe", NULL};
		char	*right_args[] = {"cat", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		t_redir	first;
		t_redir	second;
		char	*out;
		int		fd;
		int		status;

		fd = rn_test_temp(path_a);
		close(fd);
		fd = rn_test_temp(path_b);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_redir(&first, OUT_T, path_a, 1);
		rn_test_redir(&second, OUT_T, path_b, 1);
		first.next = &second;
		right.redir = &first;
		rn_test_node(&node, PIPE, NULL, &left, &right);
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

	it("lets a later file input override a heredoc inside a pipe")
	{
		char	path[] = "/tmp/minishell_pipe_hd_override_XXXXXX";
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"cat", NULL};
		char	*right_args[] = {"cat", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		t_redir	first;
		t_redir	second;
		char	*out;
		int		fd;
		int		status;

		fd = rn_test_temp(path);
		write(fd, "file\n", 5);
		close(fd);
		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_redir(&first, IN_H, "EOF", 1);
		rn_test_redir(&second, IN_F, path, 1);
		first.next = &second;
		left.redir = &first;
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_node(&node, PIPE, NULL, &left, &right);
		asserteq(rn_test_with_stdin("heredoc\nEOF\n", &node, &env, &status, &out),
			0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "file\n");
		unlink(path);
		free(out);
		env_free(&env);
	}

	it("executes nested pipe nodes")
	{
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*inner_left_args[] = {"echo", "hi", NULL};
		char	*inner_right_args[] = {"cat", NULL};
		char	*outer_right_args[] = {"wc", "-c", NULL};
		t_env	*env;
		t_exnode	inner_left;
		t_exnode	inner_right;
		t_exnode	inner;
		t_exnode	outer_right;
		t_exnode	outer;
		char	*out;
		int		status;

		env = env_init(envp);
		rn_test_node(&inner_left, CMD, inner_left_args, NULL, NULL);
		rn_test_node(&inner_right, CMD, inner_right_args, NULL, NULL);
		rn_test_node(&inner, PIPE, NULL, &inner_left, &inner_right);
		rn_test_node(&outer_right, CMD, outer_right_args, NULL, NULL);
		rn_test_node(&outer, PIPE, NULL, &inner, &outer_right);
		out = rn_test_capture_execute(&outer, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "3\n");
		free(out);
		env_free(&env);
	}

	it("returns a failing branch status when pipe redirection setup fails")
	{
		char	path[] = "/tmp/minishell_pipe_missing_input_XXXXXX";
		char	*envp[] = {"PATH=/usr/bin:/bin", NULL};
		char	*left_args[] = {"echo", "hi", NULL};
		char	*right_args[] = {"cat", NULL};
		t_env	*env;
		t_exnode	left;
		t_exnode	right;
		t_exnode	node;
		t_redir	redir;
		int		fd;
		int		status;

		fd = rn_test_temp(path);
		close(fd);
		unlink(path);
		env = env_init(envp);
		rn_test_node(&left, CMD, left_args, NULL, NULL);
		rn_test_node(&right, CMD, right_args, NULL, NULL);
		rn_test_redir(&redir, IN_F, path, 1);
		right.redir = &redir;
		rn_test_node(&node, PIPE, NULL, &left, &right);
		status = rn_execute(&node, &env);
		asserteq(status, 1);
		asserteq_str(env_get(&env, ENV_ERRCODE), "1");
		env_free(&env);
	}
}
