/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:39:48 by g-alves-          #+#    #+#             */
/*   Updated: 2025/11/17 18:36:46 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h" 

int	ft_putnbr(int n)
{
	char	string_number;
	long	big_number;
	size_t	count;

	count = 0;
	big_number = n;
	if (big_number < 0)
	{
		count += write(1, "-", 1);
		big_number = -big_number;
	}
	if (big_number > 9)
		count += ft_putnbr((big_number / 10));
	string_number = (big_number % 10) + '0';
	return (count += write (1, &string_number, 1));
}
