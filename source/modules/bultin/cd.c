/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:18:59 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/25 14:18:59 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	bin_cd(char **args, t_env **env)
{
	char	*path;
	char	*path_old;
	char	buffer[BUFFER_SIZE];

	if (!args[1])
	{
		path = env_get(env, PWD_HOME);
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (1);
		}
	}
	else
		path = args[1];
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
