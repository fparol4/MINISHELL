#include "../../headers/parser_internal.h"

void	parser_set_syntax_error(t_parser *parser, t_syntax_error type)
{
	if (!parser)
		return ;
	parser->state = TRUE;
	if (parser->ast)
		parser->ast->error_type = type;
}
