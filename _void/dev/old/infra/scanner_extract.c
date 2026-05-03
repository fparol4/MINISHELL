/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_extract.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:23:28 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/11 21:35:29 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

char	*scanner_extract(t_scanner *sc)
{
	size_t	len;

	len = sc->cursor - sc->start;
	return (ft_substr((const char *)sc->input, sc->start, len));
}
