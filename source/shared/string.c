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
