#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "./lexer.h"
# include "./errors.h"
# include "./shared.h"

# define OFFSET 8

typedef enum e_lexer_table
{
	L_NONE = 1 << (OFFSET + 0),
	L_WORD = 1 << (OFFSET + 1),
	L_REDIR_IN = 1 << (OFFSET + 2),
	L_REDIR_OUT = 1 << (OFFSET + 3),
	L_DOLLAR = 1 << (OFFSET + 4),
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
void			update_quote_state(unsigned int props, unsigned int *state);
void			init_token_table(t_char_table *table);
int				init_lexer_variable(t_manager **manager, t_rules **rules);
void			define_rules(t_rules *rules);
t_token_type	define_type(char *type);
t_node			*lx_node_new(void *content);
void			lx_list_add_back(t_manager *manager, t_node *new_node);
t_list_token	*get_word(t_manager *manager, t_scanner *input,
					t_rules *rules);
t_list_token	*get_operator(t_manager *manager, t_scanner *input,
					t_rules *rules);

#endif
