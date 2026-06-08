/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:43:51 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:43:51 by g-alves-         ###   ########.fr       */
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
