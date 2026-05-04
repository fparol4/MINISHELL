# ifndef EXECM_H
#  define EXECM_H

# include "../types/envm.h"
# include "../types/minishell.h"

char	*execm_get_path(t_env *env_list, const char *cmd);
int		execm_is_builtin(const char *cmd);
int		execm_run_builtin(t_cmd *cmd_node, t_env **env_list);
int		execm_exec_simple(t_cmd *cmd_node, t_env **env_list);

# endif
