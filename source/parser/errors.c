/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_parser.h"

void	parser_set_syntax_error(t_parser *parser, t_syntax_error type)
{
	if (!parser)
		return ;
	parser->state = TRUE;
	if (parser->ast)
		parser->ast->error_type = type;
}
