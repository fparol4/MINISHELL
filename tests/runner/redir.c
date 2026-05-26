#include "../tester.h"

describe(rn_redir)
{
	it("reads stdin from input redirection")
	{
		char		path[] = "/tmp/minishell_redir_in_XXXXXX";
		char		*raw[] = {"cat", NULL};
		t_env		*env;
		t_exnode	node;
		t_redir		redir;
		char		*out;
		int			fd;
		int			status;

		fd = rn_test_temp(path);
		env = env_init((char *[]){"PATH=/usr/bin:/bin", NULL});
		write(fd, "hello\n", 6);
		close(fd);
		rn_test_node(&node, CMD, raw, NULL, NULL);
		rn_test_redir(&redir, IN_F, path, 1);
		node.redir = &redir;
		out = rn_test_capture_execute(&node, &env, &status);
		assert(out != NULL);
		asserteq(status, 0);
		asserteq_str(out, "hello\n");
		unlink(path);
		free(out);
		env_free(&env);
	}

	it("writes stdout with truncation redirection")
	{
		char		path[] = "/tmp/minishell_redir_out_XXXXXX";
		char		*raw[] = {"echo", "new", NULL};
		t_env		*env;
		t_exnode	node;
		t_redir		redir;
		char		*out;
		int			fd;

		fd = rn_test_temp(path);
		env = NULL;
		write(fd, "old\n", 4);
		close(fd);
		rn_test_node(&node, CMD, raw, NULL, NULL);
		rn_test_redir(&redir, OUT_T, path, 1);
		node.redir = &redir;
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
		char		path[] = "/tmp/minishell_redir_app_XXXXXX";
		char		*raw[] = {"echo", "new", NULL};
		t_env		*env;
		t_exnode	node;
		t_redir		redir;
		char		*out;
		int			fd;

		fd = rn_test_temp(path);
		env = NULL;
		write(fd, "old\n", 4);
		close(fd);
		rn_test_node(&node, CMD, raw, NULL, NULL);
		rn_test_redir(&redir, OUT_A, path, 1);
		node.redir = &redir;
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
		t_exnode	node;
		t_redir		redir;
		char		*out;
		int			status;

		env = env_init(envp);
		rn_test_node(&node, CMD, raw, NULL, NULL);
		rn_test_redir(&redir, IN_H, "EOF", 1);
		node.redir = &redir;
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
		t_exnode	node;
		t_redir		redir;
		char		*out;
		int			status;

		env = env_init(envp);
		rn_test_node(&node, CMD, raw, NULL, NULL);
		rn_test_redir(&redir, IN_H, "EOF", 0);
		node.redir = &redir;
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
		t_exnode	node;
		t_redir		redir;

		env = env_init(envp);
		rn_test_node(&node, CMD, raw, NULL, NULL);
		rn_test_redir(&redir, OUT_T, "$FILES", 1);
		node.redir = &redir;
		asserteq(rn_execute(&node, &env), 1);
		asserteq_str(env_get(&env, ENV_ERRCODE), "1");
		env_free(&env);
	}
}
