/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_mark_start.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:23:28 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/11 19:37:39 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

size_t	scanner_mark_start(t_scanner *sc)
{
	if (!sc || !sc->input)
		return (0);
	sc->start = sc->cursor;
	return (1);
}
