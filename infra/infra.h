/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infra.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:24:52 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/01 20:22:47 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFRA_H
# define INFRA_H

# include "libft.h"
# include "../libft/linked_list/header.h"


typedef enum e_capture_status
{
	CAPTURE_OK,
	CAPTURE_FAIL,
	CAPTURE_UNCLOSED_GROUP,
}	t_capture_status;

typedef enum e_char_flag
{
	CHAR_NONE = 0,
	CHAR_SKIP = 1 << 0,
	CHAR_DELIM = 1 << 1,
	CHAR_OPEN_GROUP = 1 << 2,
	CHAR_CLOSE_GROUP = 1 << 3,
	CHAR_ESCAPE = 1 << 4,
	CHAR_OPERATOR = 1 << 5,
}	t_char_flag;

#endif