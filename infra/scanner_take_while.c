/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_take_while.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:23:28 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/11 21:39:05 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

void	scanner_take_while(t_scanner *sc, int (*f)(int))
{
	if (!sc || !sc->input)
		return ;
	while (!scanner_is_end(sc) && f((unsigned char)scanner_current(sc)))
		scanner_advance(sc);
}
