/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:34:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 16:34:16 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

static char	*rn_redir_value(char *line, int *idx, t_env **env)
{
	char	*value;

	value = exp_varvalue(line, idx, env);
	if (!value)
		return (ft_strdup("$"));
	return (ft_strdup(value));
}

static int	rn_redir_expand_value(char **out, char *line, int *i, t_env **env)
{
	char	*part;

	part = rn_redir_value(line, i, env);
	if (!part || rn_redir_append(out, part))
	{
		free(part);
		return (1);
	}
	free(part);
	return (0);
}

static int	rn_redir_expand_part(char **out, char *line, int *i, t_env **env)
{
	if (line[*i] == '$')
		return (rn_redir_expand_value(out, line, i, env));
	return (rn_redir_char(out, line[(*i)++]));
}

static char	*rn_redir_expand_line(char *line, t_env **env)
{
	char	*out;
	int		i;

	out = ft_strdup("");
	if (!out)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (rn_redir_expand_part(&out, line, &i, env))
		{
			free(out);
			return (NULL);
		}
	}
	return (out);
}

char	*rn_redir_line(char *line, t_env **env, int expand)
{
	char	*out;

	if (expand)
		out = rn_redir_expand_line(line, env);
	else
		out = ft_strdup(line);
	if (!out)
		return (NULL);
	if (rn_redir_append(&out, "\n"))
	{
		free(out);
		return (NULL);
	}
	return (out);
}
