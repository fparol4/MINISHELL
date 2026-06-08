/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:41 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:41 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_bin.h"

static char	*getpath(char **args, t_env **env)
{
	char	*path;

	if (!args[0])
	{
		path = env_get(env, PWD_HOME);
		if (!path)
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (path);
	}
	if (ft_strcmp(args[0], "-") == 0)
	{
		path = env_get(env, ENV_OPWD);
		if (!path)
			path = env_get(env, "OPWD");
		if (!path)
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
		else
			ft_putendl_fd(path, 1);
		return (path);
	}
	return (args[0]);
}

int	bin_cd(char **args, t_env **env)
{
	char	*path;
	char	*path_old;
	char	buffer[BUFFER_SIZE];

	if (args[0] && args[1])
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	path = getpath(args, env);
	if (!path)
		return (1);
	path_old = env_get(env, ENV_PWD);
	if (chdir(path) != 0)
	{
		perror("minishell: cd");
		return (1);
	}
	if (path_old)
		env_set(env, ENV_OPWD, path_old);
	if (getcwd(buffer, BUFFER_SIZE))
		env_set(env, ENV_PWD, buffer);
	return (0);
}
