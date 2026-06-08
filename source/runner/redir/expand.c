/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 12:55:12 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 12:55:12 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_redir.h"

static char	*redir_value_special(int *idx, t_env **env)
{
	char	*value;

	*idx += 2;
	value = env_get(env, ENV_ERRCODE);
	if (!value)
		return (ft_strdup("0"));
	return (ft_strdup(value));
}

static char	*rn_redir_value(char *line, int *idx, t_env **env)
{
	char	*key;
	char	*value;
	int		start;

	if (line[*idx + 1] == '?')
		return (redir_value_special(idx, env));
	if (!sh_varstart(line[*idx + 1]))
		return ((*idx)++, ft_strdup("$"));
	start = ++(*idx);
	while (line[*idx] && sh_varchar(line[*idx]))
		(*idx)++;
	key = ft_substr(line, start, *idx - start);
	if (!key)
		return (NULL);
	value = env_get(env, key);
	free(key);
	if (!value)
		return (ft_strdup(""));
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
		if (line[i] != '$' && rn_redir_char(&out, line[i++]))
			return (free(out), NULL);
		else if (line[i] == '$')
		{
			part = rn_redir_value(line, &i, env);
			if (!part || rn_redir_append(&out, part))
				return (free(part), free(out), NULL);
			free(part);
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
		return (free(out), NULL);
	return (out);
}
