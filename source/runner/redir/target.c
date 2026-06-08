/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

static char	*rn_redir_target(t_parser_redir *redir, t_env **env)
{
	char	*raw[2];
	char	**expanded;
	char	*target;

	if (redir->type == REDIR_HEREDOC)
		return (sh_quote_remove(redir->file));
	raw[0] = redir->file;
	raw[1] = NULL;
	expanded = rn_expand(raw, env);
	if (!expanded)
		return (NULL);
	if (!expanded[0] || expanded[1])
		return (sh_err2(NULL, redir->file, "ambiguous redirect"),
			sh_freeargs(expanded), NULL);
	target = ft_strdup(expanded[0]);
	sh_freeargs(expanded);
	return (target);
}

static int	rn_redir_open(char *target, t_parser_redir_type type)
{
	if (type == REDIR_IN)
		return (open(target, O_RDONLY));
	if (type == REDIR_OUT)
		return (open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	return (open(target, O_WRONLY | O_CREAT | O_APPEND, 0644));
}

int	rn_redir_fd(t_parser_redir *redir, t_env **env)
{
	char	*target;
	int		fd;

	target = rn_redir_target(redir, env);
	if (!target)
		return (-1);
	if (redir->type == REDIR_HEREDOC)
		fd = rn_redir_heredoc(target, env, redir->expand);
	else
		fd = rn_redir_open(target, redir->type);
	if (fd == -1 && redir->type != REDIR_HEREDOC)
		sh_err2(NULL, target, "open failed");
	free(target);
	return (fd);
}
