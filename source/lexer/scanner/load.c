/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:56 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:56 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_scanner.h"

void	scanner_init(t_scanner *sc, const char *input)
{
	if (!sc || !input)
		return ;
	sc->input = (char *)input;
	sc->cursor = 0;
	sc->len = ft_strlen(sc->input);
	sc->start = 0;
}
