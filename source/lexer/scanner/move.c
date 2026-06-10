/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   move.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 16:34:14 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 16:34:14 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_scanner.h"

void	scanner_advance(t_scanner *sc)
{
	if (sc && sc->input && sc->cursor < sc->len)
		sc->cursor++;
}

int	scanner_is_end(t_scanner *sc)
{
	if (!sc || !sc->input)
		return (1);
	return (sc->cursor >= sc->len);
}

size_t	scanner_mark_start(t_scanner *sc)
{
	if (!sc || !sc->input)
		return (0);
	sc->start = sc->cursor;
	return (1);
}

void	scanner_until(t_scanner *sc, int (*f)(int))
{
	if (!sc || !sc->input || !f)
		return ;
	while (!scanner_is_end(sc) && f(scanner_current(sc)))
		scanner_advance(sc);
}
