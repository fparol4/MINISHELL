/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_puthex.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 16:56:04 by g-alves-          #+#    #+#             */
/*   Updated: 2025/11/17 17:58:05 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h" 

static int	convert_hex(unsigned int address, char *base);

int	ft_puthex(char	*base, va_list args)
{
	unsigned int	address;

	address = va_arg(args, unsigned int);
	return (convert_hex(address, base));
}

static int	convert_hex(unsigned int address, char *base)
{
	size_t	count;

	count = 0;
	if (address >= 16)
		count += convert_hex(address / 16, base);
	return (count += write(1, &base[address % 16], 1));
}
