/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_current.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:21:33 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/23 13:28:09 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

unsigned char	scanner_current(t_scanner *sc)
{
	if (!sc || !sc->input)
		return ((unsigned char)'\0');
	return ((unsigned char)sc->input[sc->cursor]);
}
