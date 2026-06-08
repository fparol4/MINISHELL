/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:16 by fcardozo         ###   ########.fr       */
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
			return (free(fallback), NULL);
		if (rn_path_candidate(full, &fallback))
			return (free(fallback), full);
	}
	return (fallback);
}

static char	*rn_path_search(char **args, t_env **env)
{
	char	**dirs;
	char	*path;
	char	*found;

	if (ft_strchr(args[0], '/'))
		return (ft_strdup(args[0]));
	path = env_get(env, "PATH");
	if (!path || !*path)
		return (NULL);
	dirs = ft_split(path, ':');
	if (!dirs)
		return (NULL);
	found = path_search_dirs(dirs, args[0]);
	rn_path_free(dirs);
	return (found);
}

char	*rn_path(char **args, t_env **env)
{
	if (!args || !args[0])
		return (NULL);
	return (rn_path_search(args, env));
}
