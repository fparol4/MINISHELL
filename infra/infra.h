/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infra.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:24:52 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/10 17:37:40 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INFRA_H
# define INFRA_H

# include "../libft/libft.h"
# include "../libft/linked_list/header.h"


typedef enum e_capture_status
{
	CAPTURE_FAIL,
	CAPTURE_OK,
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

typedef struct s_scanner
{
	char		*input;
	size_t		cursor;
	size_t		len;
}	t_scanner;

void	scanner_init(t_scanner *sc, const char *input);
void	scanner_advance(t_scanner *sc);
char	scanner_current(t_scanner *sc);
char	scanner_peek(t_scanner *sc, size_t offset);
int		scanner_is_end(t_scanner *sc);
size_t	scanner_position(t_scanner *sc);


#endif