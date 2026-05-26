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
#include <sys/stat.h>
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

static int	rn_path_match(char *full)
{
	struct stat	st;

	if (stat(full, &st) == -1)
		return (0);
	if (S_ISDIR(st.st_mode))
		return (0);
	return (access(full, X_OK) == 0);
}

static char	*rn_path_search(char **args, t_env **env)
{
	char	**dirs;
	char	*path;
	char	*full;
	char	*fallback;
	int		i;

	if (ft_strchr(args[0], '/'))
		return (ft_strdup(args[0]));
	path = env_get(env, "PATH");
	if (!path || !*path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	fallback = NULL;
	i = 0;
	while (dirs[i])
	{
		full = rn_path_join(dirs[i++], args[0]);
		if (!full)
			return (free(fallback), rn_path_free(dirs), NULL);
		if (rn_path_match(full))
			return (free(fallback), rn_path_free(dirs), full);
		if (!fallback && access(full, F_OK) == 0)
			fallback = full;
		else
			free(full);
	}
	rn_path_free(dirs);
	return (fallback);
}

char	*rn_path(char **args, t_env **env)
{
	if (!args || !args[0])
		return (NULL);
	return (rn_path_search(args, env));
}
