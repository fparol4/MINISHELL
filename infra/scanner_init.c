/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:20:30 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/10 17:41:43 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

void	scanner_init(t_scanner *sc, const char *input)
{
	if (!sc || !input)
		return ;
	sc->input = (char *)input;
	sc->cursor = 0;
	sc->len = ft_strlen(sc->input);
}
