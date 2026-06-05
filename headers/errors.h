/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:54:19 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 18:54:19 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_H
# define ERRORS_H

typedef enum e_syntax_error
{
	SNTX_OK,
	SNTX_UNCLOSED_QUOTE,
	SNTX_PIPE_START,
	SNTX_PIPE_END,
	SNTX_EMPTY_CMD,
	SNTX_REDIR_NO_TARGET
}	t_syntax_error;

void	sh_stxerr(t_syntax_error type);

#endif
