/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_current.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:21:33 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/03 16:48:39 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

char	scanner_current(t_scanner *sc)
{
	int	index;

	index = 0;
	if (!sc->input)
		return (NULL);
	return (sc->input[0]);
}
