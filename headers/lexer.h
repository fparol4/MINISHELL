#ifndef LEXER_H
# define LEXER_H

# include "./scanner.h"

typedef struct s_node
{
	void			*content;
	struct s_node	*next;
	struct s_node	*prev;
}	t_node;

typedef struct s_manager
{
	t_node	*head;
	t_node	*tail;
}	t_manager;

typedef enum e_token_type
{
	TOKEN_NONE,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_APPEND,
	TOKEN_HEREDOC,
	TOKEN_WORD
}	t_token_type;

typedef struct s_list_token
{
	char			*value;
	t_token_type	type;
	t_bool			expand;
}	t_list_token;

t_manager	*lexer_control(t_scanner *scanner);
void		lexer_free(t_manager *manager);

#endif
