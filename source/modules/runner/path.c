/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/runner.h"
#include <unistd.h>

static void	rn_path_free(char **dirs)
{
	sh_freeargs(dirs);
}

static char	*rn_path_join(char *dir, char *cmd)
{
	char	*tmp;
	char	*full;

	tmp = ft_strjoin(dir, "/");
	if (!tmp)
		return (NULL);
	full = ft_strjoin(tmp, cmd);
	free(tmp);
	return (full);
}

static char	*rn_path_search(char **args, t_env **env)
{
	char	**dirs;
	char	*path;
	char	*full;
	int		i;

	if (ft_strchr(args[0], '/'))
	{
		if (access(args[0], F_OK | X_OK) == 0)
			return (ft_strdup(args[0]));
		return (NULL);
	}
	path = env_get(env, "PATH");
	if (!path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	i = 0;
	while (dirs[i])
	{
		full = rn_path_join(dirs[i++], args[0]);
		if (!full)
			return (rn_path_free(dirs), NULL);
		if (access(full, F_OK | X_OK) == 0)
			return (rn_path_free(dirs), full);
		free(full);
	}
	rn_path_free(dirs);
	return (NULL);
}

char	*rn_path(char **args, t_env **env)
{
	if (!args || !args[0])
		return (NULL);
	return (rn_path_search(args, env));
}
