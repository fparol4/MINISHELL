/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putunbr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:50:59 by g-alves-          #+#    #+#             */
/*   Updated: 2025/11/17 17:17:07 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putunbr(unsigned int n)
{
	char				string_number;
	unsigned long		big_number;
	unsigned int		count;

	count = 0;
	big_number = n;
	if (big_number > 9)
		count += ft_putnbr((big_number / 10));
	string_number = (big_number % 10) + '0';
	return (count += write (1, &string_number, 1));
}
