#ifndef LEXER_INTERNAL_H
# define LEXER_INTERNAL_H

# include "./lexer.h"
# include "./errors.h"
# include "./shared.h"

# define OFFSET 8
# define OPERATOR_MAX_LEN 2

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
	t_scanner_table	table;
	unsigned int	start_word;
	unsigned int	start_operator;
}	t_rules;

void			scanner_table_char(t_scanner_table *table);
void			scanner_table_token(t_scanner_table *table);
void			scanner_advance(t_scanner *sc);
int				scanner_isend(t_scanner *sc);
size_t			scanner_markstart(t_scanner *sc);
void			scanner_until(t_scanner *sc, int (*f)(int));
unsigned int	scanner_current(t_scanner *sc);
char			scanner_peek(t_scanner *sc, size_t offset);
char			*scanner_extract(t_scanner *sc);

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
