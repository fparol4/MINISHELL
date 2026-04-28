/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   infra.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/01 19:24:52 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/28 11:21:01 by g-alves-         ###   ########.fr       */
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

typedef enum e_char_prop
{
	P_NONE = 0,
	P_ALPHA = 1 << 0,
	P_DIGIT = 1 << 1,
	P_SPACE = 1 << 2,
	P_SQUOTE = 1 << 3,
	P_DQUOTE = 1 << 4,
	P_ESCAPE = 1 << 5,
	P_SYMBOL = 1 << 6
}	t_char_prop;

typedef struct s_scanner
{
	char		*input;
	size_t		cursor;
	size_t		len;
	size_t		start;
}	t_scanner;

typedef struct s_char_table
{
	unsigned int	props[256];
}	t_char_table;

/* ------------SCANNER INPUT----------------- */

void	scanner_init(t_scanner *sc, const char *input);
void	scanner_advance(t_scanner *sc);
char	scanner_current(t_scanner *sc);
char	scanner_peek(t_scanner *sc, size_t offset);
int		scanner_is_end(t_scanner *sc);
size_t	scanner_mark_start(t_scanner *sc);
void	scanner_take_while(t_scanner *sc, int (*f)(int));
char	*scanner_extract(t_scanner *sc);
void	init_char_table(t_char_table table);
int		state_char(int n);

#endif