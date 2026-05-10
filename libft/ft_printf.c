/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/09 10:50:18 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/06 22:15:12 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <limits.h>

int	ft_printf(const char *format, ...)
{
	char		*pointer;
	va_list		args;
	size_t		count;

	if (!format)
		return (0);
	count = 0;
	va_start(args, format);
	pointer = (char *)format;
	while (*pointer)
	{
		if (*pointer == '%')
		{
			pointer++;
			count += (validate_conversion(*(pointer), args));
			pointer++;
		}
		else
			count += write(1, pointer++, 1);
	}
	return (count);
}

int	validate_conversion(char type, va_list args)
{
	if (type == 'c' || type == 's')
		return (ft_printcstr(type, args));
	if (type == 'p')
		return (ft_pointer(va_arg(args, unsigned long)));
	if (type == 'i' || type == 'd')
		return (ft_putnbr(va_arg(args, int)));
	if (type == 'u')
		return (ft_putunbr(va_arg(args, unsigned int)));
	if (type == 'x')
		return (ft_puthex("0123456789abcdef", args));
	if (type == 'X')
		return (ft_puthex("0123456789ABCDEF", args));
	if (type == '%')
		return (write(1, "%", 1));
	return (0);
}
