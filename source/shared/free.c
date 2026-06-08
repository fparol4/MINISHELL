/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:38 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:38 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_shared.h"

void	sh_freesec(void **items)
{
	int	i;

	if (!items)
		return ;
	i = 0;
	while (items[i])
		free(items[i++]);
	free(items);
}

void	sh_freeargs(char **args)
{
	sh_freesec((void **)args);
}
