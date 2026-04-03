/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scanner_position.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/03 16:23:28 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/03 18:25:45 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "infra.h"

size_t	scanner_position(t_scanner *sc)
{
	if (!scanner_is_end)
		return (sc->cursor);
	//return (pesquisar o que retornar aqui);
}
