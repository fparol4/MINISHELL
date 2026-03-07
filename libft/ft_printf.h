/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/13 18:31:46 by g-alves-          #+#    #+#             */
/*   Updated: 2025/11/13 20:58:02 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdio.h>
# include <unistd.h>
# include <stdarg.h>

int			ft_printf(const char *format, ...);
size_t		ft_strlen(const char *s);
int			ft_puthex(char	*base, va_list args);
int			ft_printcstr(char type, va_list args);
int			ft_pointer(unsigned long pointer);
int			ft_putunbr(unsigned int n);	
int			ft_putnbr(int n);
int			validate_conversion(char type, va_list args);
int			put_hex(unsigned long long n, char *base);

#endif