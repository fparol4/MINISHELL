/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _lexer.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:56 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:56 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _LEXER_H
# define _LEXER_H

# include "../../headers/minishell.h"
# include "scanner/_scanner.h"

# define OPERATOR_MAX_LEN 2

typedef struct s_rules
{
	t_scanner_table	table;
	unsigned int	start_word;
	unsigned int	start_operator;
}	t_rules;

void			rules_init(t_rules *rules);
t_list_token	*extract_word(t_manager *manager, t_scanner *input,
					t_rules *rules);
t_list_token	*extract_operator(t_manager *manager, t_scanner *input,
					t_rules *rules);
void			token_free(void *content);
t_list_token	*token_add(t_manager *manager, const char *value,
					t_token_type type);
int				state_machine(t_manager *manager, t_scanner *input,
					t_rules *rules);
void			lexer_free(t_manager *manager);

#endif
