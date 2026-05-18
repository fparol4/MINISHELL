# ifndef REDIR_H
#  define REDIR_H

# include "../types/envm.h"
# include "../types/minishell.h"

int	redir_exec_in(t_cmd *cmd_node, t_env **env_list);
int	redir_exec_out(t_cmd *cmd_node, t_env **env_list);
int	redir_exec_append(t_cmd *cmd_node, t_env **env_list);

# endif
