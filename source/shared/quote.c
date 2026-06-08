/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:14 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:14 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_shared.h"

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

static int	quote_should_copy(char c, char *quote)
{
	if (!*quote && (c == '\'' || c == '"'))
	{
		*quote = c;
		return (0);
	}
	if (*quote && c == *quote)
	{
		*quote = '\0';
		return (0);
	}
	return (1);
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
	while (s[i])
	{
		if (quote_should_copy(s[i], &quote))
			out[j++] = s[i];
		i++;
	}
	out[j] = '\0';
	return (out);
}
