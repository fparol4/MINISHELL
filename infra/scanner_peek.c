/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_peek.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:21:52 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/03 18:21:40 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

char	scanner_peek(t_scanner *sc, size_t offset)
{
	size_t	pos_current;

	pos_current = scanner_position(sc);
	if (sc && sc->input)
	{
		if ((offset + pos_current) < sc->len)
			return (sc->input[offset + pos_current]);
	}
	return ('\0');
}
