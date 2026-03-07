/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/19 13:41:41 by g-alves-          #+#    #+#             */
/*   Updated: 2025/09/01 15:01:24 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	memory_size;
	void	*allocated_memory;

	memory_size = nmemb * size;
	if (nmemb == 0 || size == 0)
		memory_size = 1;
	if (((nmemb != 0) && (size != 0)) && (memory_size / size) != nmemb)
		return (NULL);
	allocated_memory = malloc(memory_size);
	if (!allocated_memory)
		return (NULL);
	ft_bzero(allocated_memory, memory_size);
	return (allocated_memory);
}
