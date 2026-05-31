#include "../../headers/parser_internal.h"

static size_t	str_array_len(char **arr)
{
	size_t	count;

	count = 0;
	if (arr)
		while (arr[count])
			count++;
	return (count);
}

void	parser_start_command(t_parser *parser)
{
	if (!parser)
		return ;
	if (!parser->current_cmd)
		parser->current_cmd = ft_calloc(1, sizeof(t_simple));
	if (!parser->current_cmd)
	{
		parser->state = TRUE;
		return ;
	}
	parser->current_cmd->args = NULL;
	parser->current_cmd->redirs = NULL;
}

void	parser_finish_command(t_parser *parser)
{
	if (!parser || !parser->current_cmd)
		return ;
	if (parser->state)
	{
		parser_free_simple(parser->current_cmd);
		parser->current_cmd = NULL;
		return ;
	}
	if (!parser->current_cmd->args && !parser->current_cmd->redirs)
	{
		parser_set_syntax_error(parser, SNTX_EMPTY_CMD);
		parser_free_simple(parser->current_cmd);
		parser->current_cmd = NULL;
		return ;
	}
	parser->current_cmd = NULL;
}

void	parser_add_arg(t_parser *parser, char *arg, t_bool expand)
{
	size_t	count;
	char	**new;

	if (!parser || !arg)
		return ;
	if (!parser->current_cmd)
		parser_start_command(parser);
	if (!parser->current_cmd)
		return ;
	if (expand)
		parser->current_cmd->expand = TRUE;
	count = str_array_len(parser->current_cmd->args);
	new = dynarray_append(parser->current_cmd->args, sizeof(char *), &count);
	if (!new)
		return ;
	new[count - 1] = arg;
	new[count] = NULL;
	parser->current_cmd->args = new;
}

void	parser_add_redir(t_parser *parser, t_parser_redir_type type,
	char *word, t_bool expand)
{
	t_parser_redir	*new;

	if (!parser || !word)
		return ;
	if (!parser->current_cmd)
		parser_start_command(parser);
	if (!parser->current_cmd)
		return ;
	new = dynarray_append(parser->current_cmd->redirs,
			sizeof(t_parser_redir),
			(size_t *)&parser->current_cmd->redir_count);
	if (!new)
		return ;
	new[parser->current_cmd->redir_count - 1].type = type;
	new[parser->current_cmd->redir_count - 1].file = word;
	new[parser->current_cmd->redir_count - 1].expand = expand;
	parser->current_cmd->redirs = new;
}
