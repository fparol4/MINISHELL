/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_internal.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/23 16:59:05 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/23 18:21:06 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "../includes/public/lexer.h"
# include "../infra/infra.h"

# define OFFSET 8

typedef struct s_char_table	t_char_table;

typedef enum e_lexer_table
{
	L_NONE = 1 << (OFFSET + 0),
	L_WORD = 1 << (OFFSET + 1),
	L_REDIR_IN = 1 << (OFFSET + 2),
	L_REDIR_OUT = 1 << (OFFSET + 3),
	L_DOLAR = 1 << (OFFSET + 4),
	L_SQUOTE = 1 << (OFFSET + 5),
	L_DQUOTE = 1 << (OFFSET + 6),
	L_PIPE = 1 << (OFFSET + 7)
}	t_lexer;

typedef struct s_rules
{
	t_char_table	table;
	unsigned int	start_word;
	unsigned int	start_operator;
}	t_rules;

t_list_token	*add_token_to_list(t_manager *manager, char *value,
					t_token_type type);
char			is_quote(char *scanner, char quote);
char			*ft_remove_char(char *str, char c);
void			init_token_table(t_char_table *table);
void			update_quote_state(unsigned int props, unsigned int *state);
int				init_lexer_variable(t_manager **manager, t_rules **rules);
void			define_rules(t_rules *rules);
t_token_type	define_type(char *type);

#endif