/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   resolve.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:15 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:15 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_external.h"

static int	rn_ext_slash(char *cmd)
{
	return (cmd && ft_strchr(cmd, '/'));
}

t_ext_kind	rn_ext_classify(char *path)
{
	struct stat	st;

	if (stat(path, &st) == -1)
	{
		if (errno == ENOENT || errno == ENOTDIR)
			return (EXT_NOT_FOUND);
		return (EXT_DENIED);
	}
	if (S_ISDIR(st.st_mode))
		return (EXT_DIR);
	if (access(path, X_OK) == -1)
		return (EXT_DENIED);
	return (EXT_READY);
}

int	rn_ext_report(char *path, t_ext_kind kind)
{
	if (kind == EXT_NOT_FOUND)
		return (sh_err2(NULL, path, "No such file or directory"), 127);
	if (kind == EXT_DIR)
		return (sh_err2(NULL, path, "Is a directory"), 126);
	return (sh_err2(NULL, path, "Permission denied"), 126);
}

int	rn_ext_resolve(char **args, t_env **env, char **path)
{
	t_ext_kind	kind;

	*path = NULL;
	if (rn_ext_slash(args[0]))
		*path = ft_strdup(args[0]);
	else
		*path = rn_path(args, env);
	if (!*path && rn_ext_slash(args[0]))
		return (1);
	if (!*path)
		return (sh_err2(NULL, args[0], "command not found"), 127);
	kind = rn_ext_classify(*path);
	if (kind == EXT_READY)
		return (0);
	return (rn_ext_report(*path, kind));
}
