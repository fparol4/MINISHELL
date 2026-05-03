#ifndef MINISHELL_H
# define MINISHELL_H

typedef enum e_cmd_type
{
	CMD_COMMAND,
	CMD_PIPE,
	CMD_AND,
	CMD_OR
}	t_cmd_type;

typedef struct s_cmd
{
	t_cmd_type		type;
	char			**args;
	struct s_cmd	*left;
	struct s_cmd	*right;
}	t_cmd;

#endif
