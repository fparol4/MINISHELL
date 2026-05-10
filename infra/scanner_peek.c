/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_peek.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:21:52 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/09 04:45:06 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

char	scanner_peek(t_scanner *sc, size_t offset)
{
	if (!sc || !sc->input)
		return ('\0');
	if (sc && sc->input)
	{
		if ((offset + sc->cursor) < sc->len)
			return (sc->input[offset + sc->cursor]);
	}
	return ('\0');
}
