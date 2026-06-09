/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _bin.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:58 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:58 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _BIN_H
# define _BIN_H

# include "../../headers/minishell.h"

int		bin_cd(char **args, t_env **env);
int		bin_echo(char **args, t_env **env);
int		bin_env(char **args, t_env **env);
int		bin_exit(char **args, t_env **env);
int		bin_export(char **args, t_env **env);
int		bin_pwd(char **args, t_env **env);
int		bin_unset(char **args, t_env **env);
int		export_print(t_env **env);
int		rn_status_get(t_env **env);
char	**export_getkeys(t_env **env, int *size_out);

#endif
