/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_list.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 19:28:41 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/14 20:18:26 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"
#include "../libft.h"

void	ft_print_list(void *content)
{
	ft_printf("Conteudo do seu nó é: %s\n", (char *)content);
}
