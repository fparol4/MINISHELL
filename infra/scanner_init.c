/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_init.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:20:30 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/03 16:55:33 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

void	scanner_init(t_scanner *sc, const char *input)
{
	if (sc && input)
	{
		sc->input = malloc((ft_strlen(input) + 1) * sizeof(char));
		if (!sc->input)
			return ;
	}
}
