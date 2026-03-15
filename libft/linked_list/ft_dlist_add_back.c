/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dlist_add_back.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 05:17:56 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/14 20:11:29 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

void	ft_dlist_add_back(t_manager *manager, t_node *new_node)
{
	if (!manager)
		return ;
	if (!manager->head)
	{
		manager->head = new_node;
		manager->tail = new_node;
	}
	else
	{
		manager->tail->next = new_node;
		new_node->prev = manager->tail;
		manager->tail = new_node;
	}
}
