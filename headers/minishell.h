#ifndef MINISHELL_H
#define MINISHELL_H

#include <unistd.h>
#include <stdio.h>

#include "../libraries/libft/libft.h"
#include "./shared.h"
#include "./env.h"
#include "./builtin.h"

# define BUFFER_SIZE 4096

# define  MSG_MINISHELL "minishell:"

typedef enum e_node_type
{
	CMD,
	PIPE
}	t_node_type;

typedef enum e_redir_type
{
	IN_F,
	IN_H,
	OUT_T,
	OUT_A
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char			*target;
	int				expand;
	struct s_redir	*next;
}	t_redir;

typedef struct s_exnode
{
	t_node_type		type;
	char			**args;
	t_redir			*redir;
	struct s_exnode	*left;
	struct s_exnode	*right;
}	t_exnode;

#endif
