/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 18:46:47 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 18:46:47 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/minishell.h"

int	sh_isspace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	sh_varstart(char c)
{
	int	is_start;

	is_start = (ft_isalpha(c) || c == '_');
	return (is_start);
}

int	sh_varchar(char c)
{
	int	is_char;

	is_char = (ft_isalnum(c) || c == '_');
	return (is_char);
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
