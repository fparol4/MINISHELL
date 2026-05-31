#include "../../headers/lexer_internal.h"

static void	free_token(void *content)
{
	t_list_token	*token;

	token = (t_list_token *)content;
	if (token)
	{
		free(token->value);
		free(token);
	}
}

void	lexer_free(t_manager *manager)
{
	t_node	*current;
	t_node	*next;

	if (!manager)
		return ;
	current = manager->head;
	while (current)
	{
		next = current->next;
		free_token(current->content);
		free(current);
		current = next;
	}
	free(manager);
}

t_list_token	*add_token_to_list(t_manager *manager, char *value,
					t_token_type type)
{
	t_node			*node;
	t_list_token	*token;

	token = malloc(sizeof(t_list_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
		return (free(token), NULL);
	token->type = type;
	token->expand = (ft_strchr(value, '$') != NULL);
	node = lx_node_new(token);
	if (!node)
		return (free(token->value), free(token), NULL);
	lx_list_add_back(manager, node);
	return (token);
}
