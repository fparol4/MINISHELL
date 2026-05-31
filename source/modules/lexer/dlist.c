#include "../../headers/lexer_internal.h"

t_node	*lx_node_new(void *content)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		return (NULL);
	node->content = content;
	node->next = NULL;
	node->prev = NULL;
	return (node);
}

void	lx_list_add_back(t_manager *manager, t_node *new_node)
{
	if (!manager)
		return ;
	new_node->next = NULL;
	if (!manager->head)
	{
		new_node->prev = NULL;
		manager->head = new_node;
		manager->tail = new_node;
	}
	else
	{
		new_node->prev = manager->tail;
		manager->tail->next = new_node;
		manager->tail = new_node;
	}
}
