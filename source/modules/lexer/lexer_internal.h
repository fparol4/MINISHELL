/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:40:46 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:40:46 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "scanner/scanner.h"
# include "../../../headers/lexer.h"
# include "../../../headers/errors.h"
# include "../../../headers/shared.h"

# define OPERATOR_MAX_LEN 2

typedef struct s_rules
{
	t_scanner_table	table;
	unsigned int	start_word;
	unsigned int	start_operator;
}	t_rules;

/* rules.c */
void			rules_init(t_rules *rules);

/* extract.c */
t_list_token	*extract_word(t_manager *manager, t_scanner *input,
					t_rules *rules);
t_list_token	*extract_operator(t_manager *manager, t_scanner *input,
					t_rules *rules);

/* token.c */
void			token_free(void *content);
t_list_token	*token_add(t_manager *manager, const char *value,
					t_token_type type);

/* state.c */
int				state_machine(t_manager *manager, t_scanner *input,
					t_rules *rules);

/* lexer.c */
void			lexer_free(t_manager *manager);

#endif
