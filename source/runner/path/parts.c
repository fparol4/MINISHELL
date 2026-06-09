/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parts.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:09:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:09:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_path.h"

void	rn_path_free(char **dirs)
{
	sh_freeargs(dirs);
}

char	*rn_path_join(char *dir, char *cmd)
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

int	rn_path_match(char *full)
{
	struct stat	st;

	if (stat(full, &st) == -1)
		return (0);
	if (S_ISDIR(st.st_mode))
		return (0);
	return (access(full, X_OK) == 0);
}

int	rn_path_candidate(char *full, char **fallback)
{
	if (rn_path_match(full))
		return (1);
	if (!*fallback && access(full, F_OK) == 0)
	{
		*fallback = full;
		return (0);
	}
	free(full);
	return (0);
}
