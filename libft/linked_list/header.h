/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/10 12:42:57 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/14 21:46:57 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LINKED_LIST_H
# define LINKED_LIST_H

# include <stdio.h>
# include <stdlib.h>
# include <string.h>

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

typedef struct s_node
{
	void			*content;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_manager
{
	t_node	*head;
	t_node	*tail;
}	t_manager;

// SIMPLE LINKED LIST
t_list	*ft_lst_new(void *content);
void	ft_lst_add_front(t_list **lst, t_list *new);
int		ft_lst_size(t_list *lst);
t_list	*ft_lst_last(t_list *lst);
void	ft_lst_add_back(t_list **lst, t_list *new);
void	ft_lst_del_one(t_list *lst, void (*del)(void *));
void	ft_lst_clear(t_list **lst, void (*del)(void *));
void	ft_print_list(void *content);


// DOUBLE LINKED LIST
void	ft_dlist_add_back(t_manager *manager, t_node *new_node);
t_node	*ft_create_node(void *content);

#endif