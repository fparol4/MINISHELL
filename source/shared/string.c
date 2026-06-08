/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:14 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:14 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_shared.h"

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
