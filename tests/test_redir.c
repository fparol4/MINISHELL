# include "snow.h"
# include "envm.h"
# include "execm.h"
# include "pipeline.h"
# include "redir.h"
# include "libft.h"
# include <stdlib.h>
# include <string.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/wait.h>

static t_cmd	*make_cmd(char **args)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	node->type = CMD_COMMAND;
	node->args = args;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

static t_cmd	*make_redir(t_cmd_type type, char *target, t_cmd *left)
{
	t_cmd	*node;
	char	**args;

	args = malloc(sizeof(char *) * 2);
	args[0] = target;
	args[1] = NULL;
	node = malloc(sizeof(t_cmd));
	node->type = type;
	node->args = args;
	node->left = left;
	node->right = NULL;
	return (node);
}

static void	free_redir(t_cmd *redir)
{
	free(redir->args);
	free(redir->left);
	free(redir);
}

static char	*read_file(const char *path)
{
	int		fd;
	char	buf[4096];
	ssize_t	n;
	char	*out;

	fd = open(path, O_RDONLY);
	if (fd == -1)
		return (NULL);
	n = read(fd, buf, sizeof(buf) - 1);
	close(fd);
	if (n < 0)
		n = 0;
	buf[n] = '\0';
	out = strdup(buf);
	return (out);
}

describe(redir_exec_out)
{
	it("creates file and writes echo output")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);

		unlink("/tmp/test_redir_out.txt");
		char	*echo_args[] = {"echo", "hello", NULL};
		t_cmd	*cmd  = make_cmd(echo_args);
		t_cmd	*node = make_redir(CMD_REDIR_OUT,
				"/tmp/test_redir_out.txt", cmd);

		asserteq(redir_exec_out(node, &env), 0);

		char	*content = read_file("/tmp/test_redir_out.txt");
		asserteq_str(content, "hello\n");
		free(content);

		free_redir(node);
		envm_free(&env);
	}

	it("truncates existing file")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);

		char	*echo_args[] = {"echo", "world", NULL};
		t_cmd	*cmd  = make_cmd(echo_args);
		t_cmd	*node = make_redir(CMD_REDIR_OUT,
				"/tmp/test_redir_out.txt", cmd);

		redir_exec_out(node, &env);
		char	*content = read_file("/tmp/test_redir_out.txt");
		asserteq_str(content, "world\n");
		free(content);

		free_redir(node);
		envm_free(&env);
	}
}

describe(redir_exec_append)
{
	it("appends to existing file")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);

		char	*echo_args[] = {"echo", "appended", NULL};
		t_cmd	*cmd  = make_cmd(echo_args);
		t_cmd	*node = make_redir(CMD_REDIR_APPEND,
				"/tmp/test_redir_out.txt", cmd);

		asserteq(redir_exec_append(node, &env), 0);

		char	*content = read_file("/tmp/test_redir_out.txt");
		assert(strstr(content, "world\n") != NULL);
		assert(strstr(content, "appended\n") != NULL);
		free(content);

		free_redir(node);
		envm_free(&env);
	}
}

describe(redir_exec_in)
{
	it("feeds file as stdin to cat")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);

		/* capture cat stdout via pipe */
		int		pfd[2];
		pipe(pfd);

		char	*cat_args[] = {"cat", NULL};
		t_cmd	*cmd  = make_cmd(cat_args);
		t_cmd	*node = make_redir(CMD_REDIR_IN,
				"/tmp/test_redir_out.txt", cmd);

		pid_t pid = fork();
		if (pid == 0)
		{
			dup2(pfd[1], STDOUT_FILENO);
			close(pfd[0]);
			close(pfd[1]);
			exit(redir_exec_in(node, &env));
		}
		close(pfd[1]);
		int		status;
		waitpid(pid, &status, 0);

		char	buf[4096];
		ssize_t	n = read(pfd[0], buf, sizeof(buf) - 1);
		close(pfd[0]);
		buf[n < 0 ? 0 : n] = '\0';

		assert(strstr(buf, "world") != NULL);
		asserteq(WEXITSTATUS(status), 0);

		free_redir(node);
		envm_free(&env);
	}

	it("returns 1 for non-existent file")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);

		char	*cat_args[] = {"cat", NULL};
		t_cmd	*cmd  = make_cmd(cat_args);
		t_cmd	*node = make_redir(CMD_REDIR_IN,
				"/tmp/no_such_file_redir_xyz", cmd);

		asserteq(redir_exec_in(node, &env), 1);

		free_redir(node);
		envm_free(&env);
	}
}

snow_main();
