/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:02 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 17:14:02 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_path.h"

static char	*path_search_dirs(char **dirs, char *cmd)
{
	char	*full;
	char	*fallback;
	int		i;

	fallback = NULL;
	i = 0;
	while (dirs[i])
	{
		full = rn_path_join(dirs[i++], cmd);
		if (!full)
		{
			free(fallback);
			return (NULL);
		}
		if (rn_path_candidate(full, &fallback))
		{
			free(fallback);
			return (full);
		}
	}
	return (fallback);
}

static char	*rn_path_search(char **args, t_env **env)
{
	char	**dirs;
	char	*path;
	char	*found;

	path = env_get(env, "PATH");
	if (!path || !*path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	found = path_search_dirs(dirs, args[0]);
	sh_freeargs(dirs);
	return (found);
}

char	*rn_path(char **args, t_env **env)
{
	char	*path;

	if (!args || !args[0])
		return (NULL);
	path = rn_path_search(args, env);
	return (path);
}
