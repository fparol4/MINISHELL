#include "../tester.h"

describe(rn_redir)
{
	it("reads stdin from input redirection")
	{
		char		path[] = "tests/_tmp/minishell_redir_in_XXXXXX";
		char		*raw[] = {"cat", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir		redir;
		char		*out;
		int			fd;
		int			status;

		fd = rn_test_temp(path);
		env = env_init((char *[]){"PATH=/usr/bin:/bin", NULL});
		write(fd, "hello\n", 6);
		close(fd);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_IN, path, 1);
		rn_test_attach_redirs(&node, &redir, 1);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "hello\n");
		rn_test_ensure_tmp();
		unlink(path);
		free(out);
		env_free(&env);
	}

	it("writes stdout with truncation redirection")
	{
		char		path[] = "tests/_tmp/minishell_redir_out_XXXXXX";
		char		*raw[] = {"echo", "new", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir		redir;
		char		*out;
		int			fd;

		fd = rn_test_temp(path);
		env = NULL;
		write(fd, "old\n", 4);
		close(fd);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_OUT, path, 1);
		rn_test_attach_redirs(&node, &redir, 1);
		asserteq(rn_execute(&node, &env), 0);
		out = rn_test_readfile(path);
		assert(out != NULL);
		asserteq_str(out, "new\n");
		unlink(path);
		free(out);
		env_free(&env);
	}

	it("writes stdout with append redirection")
	{
		char		path[] = "tests/_tmp/minishell_redir_app_XXXXXX";
		char		*raw[] = {"echo", "new", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir		redir;
		char		*out;
		int			fd;

		fd = rn_test_temp(path);
		env = NULL;
		write(fd, "old\n", 4);
		close(fd);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_APPEND, path, 1);
		rn_test_attach_redirs(&node, &redir, 1);
		asserteq(rn_execute(&node, &env), 0);
		out = rn_test_readfile(path);
		assert(out != NULL);
		asserteq_str(out, "old\nnew\n");
		unlink(path);
		free(out);
		env_free(&env);
	}

	it("expands heredoc content when enabled")
	{
		char		*raw[] = {"cat", NULL};
		char		*envp[] = {"PATH=/usr/bin:/bin", "USER=bar", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir		redir;
		char		*out;
		int			status;

		env = env_init(envp);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_HEREDOC, "EOF", 1);
		rn_test_attach_redirs(&node, &redir, 1);
		asserteq(rn_test_with_stdin("$USER\nEOF\n", &node, &env, &status, &out), 0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "bar\n");
		free(out);
		env_free(&env);
	}

	it("keeps heredoc content literal when expansion is disabled")
	{
		char		*raw[] = {"cat", NULL};
		char		*envp[] = {"PATH=/usr/bin:/bin", "USER=bar", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir		redir;
		char		*out;
		int			status;

		env = env_init(envp);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_HEREDOC, "EOF", 0);
		rn_test_attach_redirs(&node, &redir, 1);
		asserteq(rn_test_with_stdin("$USER\nEOF\n", &node, &env, &status, &out), 0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "$USER\n");
		free(out);
		env_free(&env);
	}

	it("fails when a redirection target expands to many args")
	{
		char		*raw[] = {"echo", "hello", NULL};
		char		*envp[] = {"FILES=a b", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir		redir;

		env = env_init(envp);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_OUT, "$FILES", 1);
		rn_test_attach_redirs(&node, &redir, 1);
		asserteq(rn_execute(&node, &env), 1);
		asserteq_str(env_get(&env, ENV_ERRCODE), "1");
		env_free(&env);
	}

	it("fails when a redirection target expands to empty")
	{
		char			*raw[] = {"echo", "hello", NULL};
		char			*envp[] = {"EMPTY=", NULL};
		t_env			*env;
		t_command		node;
		t_parser_redir	redir;
		char			*err;
		int				status;

		env = env_init(envp);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_OUT, "$EMPTY", 1);
		rn_test_attach_redirs(&node, &redir, 1);
		err = rn_test_capture_error(&node, &env, &status);
		assert(err != NULL);
		asserteq(status, 1);
		assert(strstr(err, "ambiguous redirect") != NULL);
		asserteq_str(env_get(&env, ENV_ERRCODE), "1");
		free(err);
		env_free(&env);
	}

	it("uses quoted spaces as one redirection target")
	{
		char			dir[] = "tests/_tmp/minishell_redir_space_XXXXXX";
		char			*raw[] = {"echo", "ok", NULL};
		t_env			*env;
		t_command		node;
		t_parser_redir	redir;
		char			*path;
		char			*target;
		char			*out;

		assert(rn_test_mkdtemp(dir) != NULL);
		path = ft_strjoin(dir, "/two words");
		assert(path != NULL);
		target = rn_test_quote(path);
		env = NULL;
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_OUT, target, 0);
		rn_test_attach_redirs(&node, &redir, 1);
		asserteq(rn_execute(&node, &env), 0);
		out = rn_test_readfile(path);
		assert(out != NULL);
		asserteq_str(out, "ok\n");
		unlink(path);
		rmdir(dir);
		free(out);
		free(target);
		free(path);
		env_free(&env);
	}

	it("fails when an unquoted target expands to spaces")
	{
		char			*raw[] = {"echo", "hello", NULL};
		char			*envp[] = {"TARGET=two words", NULL};
		t_env			*env;
		t_command		node;
		t_parser_redir	redir;
		char			*err;
		int				status;

		env = env_init(envp);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_OUT, "$TARGET", 1);
		rn_test_attach_redirs(&node, &redir, 1);
		err = rn_test_capture_error(&node, &env, &status);
		assert(err != NULL);
		asserteq(status, 1);
		assert(strstr(err, "ambiguous redirect") != NULL);
		free(err);
		env_free(&env);
	}

	it("fails output redirection when permission is denied")
	{
		char			dir[] = "tests/_tmp/minishell_redir_denied_XXXXXX";
		char			*raw[] = {"echo", "blocked", NULL};
		t_env			*env;
		t_command		node;
		t_parser_redir	redir;
		char			*path;
		char			*err;
		int				status;

		assert(rn_test_mkdtemp(dir) != NULL);
		path = ft_strjoin(dir, "/out");
		assert(path != NULL);
		assert(chmod(dir, 0500) == 0);
		env = NULL;
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_OUT, path, 1);
		rn_test_attach_redirs(&node, &redir, 1);
		err = rn_test_capture_error(&node, &env, &status);
		assert(err != NULL);
		asserteq(status, 1);
		assert(strstr(err, "open failed") != NULL);
		chmod(dir, 0700);
		unlink(path);
		rmdir(dir);
		free(path);
		free(err);
		env_free(&env);
	}

	it("does not execute command when input redirection is missing")
	{
		char			path[] = "tests/_tmp/minishell_redir_missing_input";
		char			*raw[] = {"echo", "should-not-run", NULL};
		t_env			*env;
		t_command		node;
		t_parser_redir	redir;
		char			*out;
		int				status;

		unlink(path);
		env = NULL;
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_IN, path, 1);
		rn_test_attach_redirs(&node, &redir, 1);
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 1);
		asserteq_str(out, "");
		free(out);
		env_free(&env);
	}

	it("consumes multiple heredocs in order and keeps the last one on stdin")
	{
		char		*raw[] = {"cat", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir	redirs[2];
		char		*out;
		int			status;

		env = env_init((char *[]){"PATH=/usr/bin:/bin", NULL});
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redirs[0], REDIR_HEREDOC, "A", 1);
		rn_test_redir(&redirs[1], REDIR_HEREDOC, "B", 1);
		rn_test_attach_redirs(&node, redirs, 2);
		asserteq(rn_test_with_stdin("first\nA\nsecond\nB\n", &node, &env, &status,
				&out), 0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "second\n");
		free(out);
		env_free(&env);
	}

	it("lets a later file input override an earlier heredoc")
	{
		char		path[] = "tests/_tmp/minishell_redir_override_in_XXXXXX";
		char		*raw[] = {"cat", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir	redirs[2];
		char		*out;
		int			fd;
		int			status;

		fd = rn_test_temp(path);
		write(fd, "file\n", 5);
		close(fd);
		env = env_init((char *[]){"PATH=/usr/bin:/bin", NULL});
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redirs[0], REDIR_HEREDOC, "EOF", 1);
		rn_test_redir(&redirs[1], REDIR_IN, path, 1);
		rn_test_attach_redirs(&node, redirs, 2);
		asserteq(rn_test_with_stdin("heredoc\nEOF\n", &node, &env, &status, &out),
			0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "file\n");
		unlink(path);
		free(out);
		env_free(&env);
	}

	it("lets a later heredoc override an earlier file input")
	{
		char		path[] = "tests/_tmp/minishell_redir_override_heredoc_XXXXXX";
		char		*raw[] = {"cat", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir	redirs[2];
		char		*out;
		int			fd;
		int			status;

		fd = rn_test_temp(path);
		write(fd, "file\n", 5);
		close(fd);
		env = env_init((char *[]){"PATH=/usr/bin:/bin", NULL});
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redirs[0], REDIR_IN, path, 1);
		rn_test_redir(&redirs[1], REDIR_HEREDOC, "EOF", 1);
		rn_test_attach_redirs(&node, redirs, 2);
		asserteq(rn_test_with_stdin("heredoc\nEOF\n", &node, &env, &status, &out),
			0);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "heredoc\n");
		unlink(path);
		free(out);
		env_free(&env);
	}

	it("warns when heredoc ends before its delimiter")
	{
		char		*raw[] = {"true", NULL};
		t_env		*env;
		t_command	node;
		t_parser_redir		redir;
		char		*err;
		int			status;

		env = env_init((char *[]){"PATH=/usr/bin:/bin", NULL});
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		rn_test_redir(&redir, REDIR_HEREDOC, "EOF", 1);
		rn_test_attach_redirs(&node, &redir, 1);
		asserteq(rn_test_with_stdin_error("partial line\n", &node, &env, &status,
				&err), 0);
		assert(err != NULL);
		asserteq(status, 0);
		assert(strstr(err, "warning: here-document delimited by end-of-file")
			!= NULL);
		assert(strstr(err, "EOF") != NULL);
		free(err);
		env_free(&env);
	}

	it("handles commands with only redirections")
	{
		char		path[] = "tests/_tmp/minishell_redir_only_XXXXXX";
		t_env		*env;
		t_command	node;
		t_parser_redir		redir;
		char		*out;
		int			fd;

		fd = rn_test_temp(path);
		write(fd, "old\n", 4);
		close(fd);
		env = NULL;
		rn_test_node(&node, PNODE_CMD, NULL, NULL, NULL);
		rn_test_redir(&redir, REDIR_OUT, path, 1);
		rn_test_attach_redirs(&node, &redir, 1);
		asserteq(rn_execute(&node, &env), 0);
		out = rn_test_readfile(path);
		assert(out != NULL);
		asserteq_str(out, "");
		unlink(path);
		free(out);
		env_free(&env);
	}
}
