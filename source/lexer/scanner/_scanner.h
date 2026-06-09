/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _scanner.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:56 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:56 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SCANNER_H
# define _SCANNER_H

# include "../../../libraries/libft/libft.h"

# define OFFSET 8

typedef enum e_table_char
{
	P_NONE = 0,
	P_ALPHA = 1 << 0,
	P_DIGIT = 1 << 1,
	P_SPACE = 1 << 2,
	P_SQUOTE = 1 << 3,
	P_DQUOTE = 1 << 4,
	P_ESCAPE = 1 << 5,
	P_SYMBOL = 1 << 6
}	t_table_char;

typedef enum e_token_table
{
	L_NONE = 1 << (OFFSET + 0),
	L_WORD = 1 << (OFFSET + 1),
	L_REDIR_IN = 1 << (OFFSET + 2),
	L_REDIR_OUT = 1 << (OFFSET + 3),
	L_DOLLAR = 1 << (OFFSET + 4),
	L_SQUOTE = 1 << (OFFSET + 5),
	L_DQUOTE = 1 << (OFFSET + 6),
	L_PIPE = 1 << (OFFSET + 7)
}					t_token_table;

typedef struct s_scanner
{
	char	*input;
	size_t	cursor;
	size_t	len;
	size_t	start;
}	t_scanner;

typedef struct s_scanner_table
{
	unsigned int	props[256];
}	t_scanner_table;

void	scanner_init(t_scanner *sc, const char *input);
void	scanner_advance(t_scanner *sc);
void	scanner_until(t_scanner *sc, int (*f)(int));
void	scanner_table_char(t_scanner_table *table);
void	scanner_table_token(t_scanner_table *table);
int		scanner_is_end(t_scanner *sc);
char	scanner_peek(t_scanner *sc, size_t offset);
char	*scanner_extract(t_scanner *sc);
size_t	scanner_mark_start(t_scanner *sc);
size_t	scanner_current(t_scanner *sc);

#endif
