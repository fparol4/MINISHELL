/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   target.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:47 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:47 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

static char	*rn_redir_target(t_parser_redir *redir, t_env **env)
{
	char	*raw[2];
	char	**expanded;
	char	*target;

	if (redir->type == REDIR_HEREDOC)
	{
		target = sh_quote_remove(redir->file);
		return (target);
	}
	raw[0] = redir->file;
	raw[1] = NULL;
	// @TODO
	expanded = rn_expand(raw, env);
	if (!expanded)
		return (NULL);
	if (!expanded[0] || expanded[1])
	{
		sh_err2(NULL, redir->file, "ambiguous redirect");
		sh_freeargs(expanded);
		return (NULL);
	}
	target = ft_strdup(expanded[0]);
	sh_freeargs(expanded);
	return (target);
}

static int	rn_redir_open(char *target, t_parser_redir_type type)
{
	int	fd;

	if (type == REDIR_IN)
	{
		fd = open(target, O_RDONLY);
		return (fd);
	}
	if (type == REDIR_OUT)
	{
		fd = open(target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		return (fd);
	}
	fd = open(target, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (fd);
}

int	rn_redir_fd(t_parser_redir *redir, t_env **env, int input_fd)
{
	char	*target;
	int		fd;

	target = rn_redir_target(redir, env);
	if (!target)
		return (REDIR_FD_ERR);
	// P1
	if (redir->type == REDIR_HEREDOC)
		fd = rn_redir_heredoc(target, env, redir->expand, input_fd);
	// P2
	else
		fd = rn_redir_open(target, redir->type);
	if (fd == REDIR_FD_ERR && redir->type != REDIR_HEREDOC)
		sh_err2(NULL, target, strerror(errno));
	free(target);
	return (fd);
}
