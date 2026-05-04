# include "snow.h"
# include "envm.h"
# include "execm.h"
# include "pipeline.h"
# include "libft.h"
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

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

static t_cmd	*make_pipe(t_cmd *left, t_cmd *right)
{
	t_cmd	*node;

	node = malloc(sizeof(t_cmd));
	node->type = CMD_PIPE;
	node->args = NULL;
	node->left = left;
	node->right = right;
	return (node);
}

describe(pipeline_exec_pipe)
{
	it("executes echo | wc -c and returns 0")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);

		char	*echo_args[] = {"echo", "hello", NULL};
		char	*wc_args[]   = {"wc", "-c", NULL};
		t_cmd	*left  = make_cmd(echo_args);
		t_cmd	*right = make_cmd(wc_args);
		t_cmd	*pipe_node = make_pipe(left, right);

		asserteq(pipeline_exec_pipe(pipe_node, &env), 0);

		free(left);
		free(right);
		free(pipe_node);
		envm_free(&env);
	}

	it("returns right child exit status (cat /no/such | true => 0)")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);

		char	*cat_args[]  = {"cat", "/no_such_file_xyz", NULL};
		char	*true_args[] = {"true", NULL};
		t_cmd	*left  = make_cmd(cat_args);
		t_cmd	*right = make_cmd(true_args);
		t_cmd	*pipe_node = make_pipe(left, right);

		asserteq(pipeline_exec_pipe(pipe_node, &env), 0);

		free(left);
		free(right);
		free(pipe_node);
		envm_free(&env);
	}

	it("does not hang on three-stage pipe")
	{
		char	*env_arr[] = {"PATH=/usr/bin:/bin", NULL};
		t_env	*env = envm_init(env_arr);

		char	*echo_args[] = {"echo", "hello world", NULL};
		char	*cat_args[]  = {"cat", NULL};
		char	*wc_args[]   = {"wc", "-w", NULL};

		t_cmd	*c1 = make_cmd(echo_args);
		t_cmd	*c2 = make_cmd(cat_args);
		t_cmd	*c3 = make_cmd(wc_args);

		t_cmd	*inner = make_pipe(c1, c2);
		t_cmd	*outer = make_pipe(inner, c3);

		asserteq(pipeline_exec_pipe(outer, &env), 0);

		free(c1);
		free(c2);
		free(c3);
		free(inner);
		free(outer);
		envm_free(&env);
	}
}

snow_main();
