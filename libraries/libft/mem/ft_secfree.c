/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_secfree.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/03 18:05:57 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/03 18:05:57 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

void	*ft_secfree(char **buffer[], int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		if (*buffer[i] != NULL)
		{
			free(*buffer[i]);
			*buffer[i] = NULL;
		}
		i++;
	}
	return (NULL);
}
