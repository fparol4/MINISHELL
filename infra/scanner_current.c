/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_current.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:21:33 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/04 14:59:54 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

unsigned int	scanner_current(t_scanner *sc)
{
	unsigned int	current;

	if (!sc || !sc->input)	
		return (0);
	current = (unsigned int)sc->input[sc->cursor];
	return (current);
}
