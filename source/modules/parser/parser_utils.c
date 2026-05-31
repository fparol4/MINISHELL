#include "../../headers/parser_internal.h"

void	parser_init(t_parser *parser, t_manager *manager)
{
	if (!parser)
		return ;
	parser->current = NULL;
	parser->token = NULL;
	parser->state = FALSE;
	parser->current_cmd = NULL;
	if (manager)
	{
		parser->current = manager->head;
		parser->token = get_token(parser->current);
	}
}

void	parser_next(t_parser *parser)
{
	if (!parser || !parser->current)
		return ;
	parser->last_type = parser->token->type;
	parser->current = parser->current->next;
	parser->token = get_token(parser->current);
}

t_bool	parser_is_end(t_parser *parser)
{
	if (!parser || !parser->current || !parser->token)
		return (TRUE);
	return (FALSE);
}

unsigned int	token_class(t_token_type type)
{
	if (type == TOKEN_WORD)
		return (C_WORD);
	if (type == TOKEN_REDIR_IN || type == TOKEN_REDIR_OUT
		|| type == TOKEN_APPEND || type == TOKEN_HEREDOC)
		return (C_REDIR);
	if (type == TOKEN_PIPE)
		return (C_PIPE);
	return (C_NONE);
}

t_list_token	*get_token(t_node *current_node)
{
	if (!current_node)
		return (NULL);
	return ((t_list_token *)current_node->content);
}
