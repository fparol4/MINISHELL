/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pointer.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:24:00 by g-alves-          #+#    #+#             */
/*   Updated: 2025/11/17 19:21:24 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h" 

int	ft_pointer(unsigned long pointer)
{
	size_t			count;

	count = 0;
	if (pointer == 0)
	{
		count += write (1, "(nil)", 5);
		return (count);
	}
	count = write(1, "0x", 2);
	return (count += put_hex(pointer, "0123456789abcdef"));
}

int	put_hex(unsigned long long n, char *base)
{
	size_t	count;

	count = 0;
	if (n >= 16)
		count += put_hex(n / 16, base);
	return (count += write(1, &base[n % 16], 1));
}
