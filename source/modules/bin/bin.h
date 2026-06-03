#ifndef BIN_H
# define BIN_H

# include "../../../headers/builtin.h"
# include "../../../headers/errors.h"
# include "../../../headers/shared.h"

/* cd.c */
int	bin_cd(char **args, t_env **env);

/* echo.c */
int	bin_echo(char **args, t_env **env);

/* env.c */
int	bin_env(char **args, t_env **env);

/* exit.c */
int	bin_exit(char **args, t_env **env);

/* export.c */
int	bin_export(char **args, t_env **env);

/* pwd.c */
int	bin_pwd(char **args, t_env **env);

/* unset.c */
int	bin_unset(char **args, t_env **env);

#endif
