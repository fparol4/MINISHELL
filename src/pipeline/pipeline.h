# ifndef PIPELINE_H
#  define PIPELINE_H

# include "../types/envm.h"
# include "../types/minishell.h"

int	pipeline_exec_node(t_cmd *cmd_node, t_env **env_list);
int	pipeline_exec_pipe(t_cmd *cmd_node, t_env **env_list);

# endif
