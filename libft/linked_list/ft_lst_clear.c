/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lst_clear.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/14 19:46:32 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/16 18:07:25 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_lst_clear(t_list 
	**lst, void (*del)(void *))
{
	t_list	*next_node;

	if (!lst || !del || !*lst)
		return ;
	while (next_node != NULL)
	{
		next_node = (*lst)->next;
		ft_lst_del_one(*lst, del);
		*lst = next_node;
	}
}