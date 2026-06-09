/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:47 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:47 by fcardozo         ###   ########.fr       */
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

static char	*rn_redir_expand_line(char *line, t_env **env)
{
	char	*out;
	char	*part;
	int		i;

	out = ft_strdup("");
	if (!out)
		return (NULL);
	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			part = rn_redir_value(line, &i, env);
			if (!part || rn_redir_append(&out, part))
			{
				free(part);
				free(out);
				return (NULL);
			}
			free(part);
		}
		else
		{
			if (rn_redir_char(&out, line[i++]))
			{
				free(out);
				return (NULL);
			}
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
