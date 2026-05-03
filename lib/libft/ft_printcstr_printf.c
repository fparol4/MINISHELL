/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printcstr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 14:49:30 by g-alves-          #+#    #+#             */
/*   Updated: 2025/11/17 17:07:10 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h" 

int	ft_printcstr(char type, va_list args)
{
	char	c;
	char	*s;

	if (type == 'c')
	{
		c = va_arg(args, int);
		write (1, &c, 1);
		return (1);
	}
	if (type == 's')
	{
		s = va_arg(args, char *);
		if (!s)
			s = "(null)";
		write (1, s, ft_strlen(s));
		return (ft_strlen(s));
	}
	return (0);
}
