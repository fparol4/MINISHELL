/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 14:06:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/shared.h"

int	sh_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	sh_varstart(char c)
{
	return (ft_isalpha(c) || c == '_');
}

int	sh_varchar(char c)
{
	return (ft_isalnum(c) || c == '_');
}

int	sh_isidentifier(char *s)
{
	int	i;

	if (!s || (!ft_isalpha(s[0]) && s[0] != '_'))
		return (0);
	i = 1;
	while (s[i])
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	sh_has_quotes(char *s)
{
	int	i;

	i = 0;
	while (s && s[i])
	{
		if (s[i] == '\'' || s[i] == '"')
			return (1);
		i++;
	}
	return (0);
}

char	*sh_quote_remove(char *s)
{
	char	*out;
	int		i;
	int		j;
	char	quote;

	if (!s)
		return (NULL);
	out = malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!out)
		return (NULL);
	i = 0;
	j = 0;
	quote = '\0';
	while (s && s[i])
	{
		if (!quote && (s[i] == '\'' || s[i] == '"'))
			quote = s[i++];
		else if (quote && s[i] == quote)
			quote = (i++, '\0');
		else
			out[j++] = s[i++];
	}
	out[j] = '\0';
	return (out);
}
