/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tester.h"

describe(rn_exec_bin)
{
	{
	char	*args[] = {"export", "FOO=bar", NULL};
	t_env	*env;
	int		status;

	it("handles builtin commands")
	{
		env = NULL;
		status = 42;
		asserteq(rn_exec_bin(args, &env, &status), 1);
		asserteq(status, 0);
		asserteq_str(env_get(&env, "FOO"), "bar");
		env_free(&env);
	}
	}
	{
	char	*args[] = {"ls", NULL};
	t_env	*env;
	int		status;

	it("returns 0 for external commands")
	{
		env = NULL;
		status = 42;
		asserteq(rn_exec_bin(args, &env, &status), 0);
		asserteq(status, 42);
	}
	}
}

describe(rn_execute_cmd)
{
	{
	char		*raw[] = {"export", "FOO=$USER", NULL};
	char		*envp[] = {"USER=bar", NULL};
	t_env		*env;
	t_command	node;

	it("expands args and runs builtin command")
	{
		env = env_init(envp);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		asserteq(rn_execute(&node, &env), 0);
		asserteq_str(env_get(&env, "FOO"), "bar");
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		env_free(&env);
	}
	}
	{
	char		*raw[] = {"true", NULL};
	char		*envp[] = {"PATH=/usr/bin:/bin", NULL};
	t_env		*env;
	t_command	node;

	it("runs external commands")
	{
		env = env_init(envp);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		asserteq(rn_execute(&node, &env), 0);
		asserteq_str(env_get(&env, ENV_ERRCODE), "0");
		env_free(&env);
	}
	}
	{
	char		*raw[] = {"not_a_real_command_xyz", NULL};
	char		*envp[] = {"PATH=/usr/bin:/bin", NULL};
	t_env		*env;
	t_command	node;

	it("returns 127 and updates status for missing external commands")
	{
		env = env_init(envp);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		asserteq(rn_execute(&node, &env), 127);
		asserteq_str(env_get(&env, ENV_ERRCODE), "127");
		env_free(&env);
	}
	}
	{
	char		*raw[] = {"./__minishell_missing_exec__", NULL};
	t_env		*env;
	t_command	node;
	char		*err;
	int			status;

	it("returns 127 for direct paths that do not exist")
	{
		env = NULL;
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		err = rn_test_capture_error(&node, &env, &status);
		assert(err != NULL);
		asserteq(status, 127);
		assert(strstr(err, "No such file or directory") != NULL);
		asserteq_str(env_get(&env, ENV_ERRCODE), "127");
		free(err);
		env_free(&env);
	}
	}
	{
	char		path[] = "tests/_tmp/minishell_exec_dir_XXXXXX";
	char		*raw[] = {path, NULL};
	t_env		*env;
	t_command	node;
	char		*err;
	int			status;

	it("returns 126 for direct directory paths")
	{
		assert(rn_test_mkdtemp(path) != NULL);
		env = NULL;
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		err = rn_test_capture_error(&node, &env, &status);
		assert(err != NULL);
		asserteq(status, 126);
		assert(strstr(err, "Is a directory") != NULL);
		asserteq_str(env_get(&env, ENV_ERRCODE), "126");
		rmdir(path);
		free(err);
		env_free(&env);
	}
	}
	{
	char		path[] = "tests/_tmp/minishell_exec_noexec_XXXXXX";
	char		*raw[] = {path, NULL};
	t_env		*env;
	t_command	node;
	char		*err;
	int			fd;
	int			status;

	it("returns 126 for direct files without execute permission")
	{
		fd = rn_test_temp(path);
		write(fd, "echo nope\n", 10);
		close(fd);
		env = NULL;
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		err = rn_test_capture_error(&node, &env, &status);
		assert(err != NULL);
		asserteq(status, 126);
		assert(strstr(err, "Permission denied") != NULL);
		asserteq_str(env_get(&env, ENV_ERRCODE), "126");
		unlink(path);
		free(err);
		env_free(&env);
	}
	}
	{
	char		path[] = "tests/_tmp/minishell_exec_format_XXXXXX";
	char		*raw[] = {path, NULL};
	t_env		*env;
	t_command	node;
	char		*err;
	int			fd;
	int			status;

	it("returns 126 for executable files with invalid format")
	{
		fd = rn_test_temp(path);
		write(fd, "plain text\n", 11);
		close(fd);
		assert(chmod(path, 0700) == 0);
		env = NULL;
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		err = rn_test_capture_error(&node, &env, &status);
		assert(err != NULL);
		asserteq(status, 126);
		assert(strstr(err, "Exec format error") != NULL);
		asserteq_str(env_get(&env, ENV_ERRCODE), "126");
		unlink(path);
		free(err);
		env_free(&env);
	}
	}
	{
	char		dir[] = "tests/_tmp/minishell_path_dir_XXXXXX";
	char		*raw[] = {"blocked", NULL};
	char		*envp[2];
	char		*path;
	char		*tmp;
	char		*err;
	char		*path_env;
	t_env		*env;
	t_command	node;
	int			fd;
	int			status;

	it("returns 126 for PATH entries without execute permission")
	{
		assert(rn_test_mkdtemp(dir) != NULL);
		tmp = ft_strjoin(dir, "/");
		assert(tmp != NULL);
		path = ft_strjoin(tmp, "blocked");
		free(tmp);
		assert(path != NULL);
		fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		assert(fd != -1);
		write(fd, "echo nope\n", 10);
		close(fd);
		path_env = ft_strjoin("PATH=", dir);
		assert(path_env != NULL);
		envp[0] = path_env;
		envp[1] = NULL;
		env = env_init(envp);
		rn_test_node(&node, PNODE_CMD, raw, NULL, NULL);
		err = rn_test_capture_error(&node, &env, &status);
		assert(err != NULL);
		asserteq(status, 126);
		assert(strstr(err, "Permission denied") != NULL);
		assert(strstr(err, path) != NULL);
		asserteq_str(env_get(&env, ENV_ERRCODE), "126");
		unlink(path);
		rmdir(dir);
		free(path_env);
		free(path);
		free(err);
		env_free(&env);
	}
	}
}
