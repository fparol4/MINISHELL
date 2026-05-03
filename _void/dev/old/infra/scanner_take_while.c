/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_take_while.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:23:28 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/23 16:24:01 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

void	scanner_take_while(t_scanner *sc, int (*f)(int))
{
	if (!sc || !sc->input)
		return ;
	while (!scanner_is_end(sc) && f(scanner_current(sc)))
		scanner_advance(sc);
}
