/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:17:15 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/04 10:54:30 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static long	is_digit(long number);
static char	*pass_string(char *string_number, long number, long count_string,
				int signal);
static long	pass_number(long number);
static int	flag(int n, long *number);

char	*ft_itoa(int n)
{
	char	*string_number;
	long	count_string;
	long	number;
	int		signal;

	number = (long)n;
	signal = flag(n, &number);
	count_string = 1;
	while (is_digit(number) > 0)
	{
		number = number / 10;
		count_string++;
	}
	if (signal)
		string_number = ft_calloc((count_string + 2), sizeof(char));
	else
		string_number = ft_calloc((count_string + 1), sizeof(char));
	if (!string_number)
		return (NULL);
	signal = flag(n, &number);
	pass_string(string_number, number, count_string, signal);
	return (string_number);
}

static int	flag(int n, long *number)
{
	if (n < 0)
	{
		*number = (long)n * -1;
		return (1);
	}
	else
	{
		*number = n;
		return (0);
	}
}

static char	*pass_string(char *string_number, long number, long count_string,
				int signal)
{
	if (signal)
	{
		string_number[0] = '-';
		count_string++;
	}
	string_number[count_string--] = '\0';
	while (count_string >= 0)
	{
		if (signal && count_string == 0)
			break ;
		string_number[count_string] = '0' + pass_number(number);
		number = number / 10;
		count_string--;
	}
	return (string_number);
}

static long	is_digit(long number)
{
	long	remainder;

	remainder = 0;
	if (number >= 0 && number < 10)
		return (0);
	if (number > 9)
	{
		remainder = number % 10;
		return (1);
	}
	return (remainder);
}

static long	pass_number(long number)
{
	long	remainder;

	remainder = 0;
	if (number >= 0 && number < 10)
		return (number);
	if (number > 9)
	{
		remainder = number % 10;
		return (remainder);
	}
	return (remainder);
}

// int	main(void)
// {
// 	printf("int convertido em string: %s\n", ft_itoa(-2147483648));
// 	return (0);
// }
