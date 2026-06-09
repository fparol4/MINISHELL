/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:56 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:56 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_scanner.h"

size_t	scanner_current(t_scanner *sc)
{
	if (!sc || !sc->input || sc->cursor >= sc->len)
		return (0);
	return ((unsigned char)sc->input[sc->cursor]);
}

char	scanner_peek(t_scanner *sc, size_t offset)
{
	if (!sc || !sc->input)
		return ('\0');
	if (sc->cursor + offset < sc->len)
		return (sc->input[sc->cursor + offset]);
	return ('\0');
}

char	*scanner_extract(t_scanner *sc)
{
	if (!sc || !sc->input || sc->cursor < sc->start)
		return (NULL);
	return (ft_substr(sc->input, sc->start, sc->cursor - sc->start));
}
