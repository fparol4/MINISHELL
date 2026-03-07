/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putnbr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/02 15:15:33 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/03 13:03:11 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putnbr_fd(int n, int fd)
{
	char	string_number;
	long	big_number;

	big_number = n;
	if (big_number < 0)
	{
		write(fd, "-", 1);
		big_number = -big_number;
	}
	if (big_number >= 0 && big_number <= 9)
	{
		string_number = big_number + '0';
		write(fd, &string_number, 1);
	}
	if (big_number > 9)
	{
		ft_putnbr_fd((big_number / 10), fd);
		string_number = (big_number % 10) + '0';
		write (fd, &string_number, 1);
	}
}
