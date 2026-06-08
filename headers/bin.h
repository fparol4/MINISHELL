/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIN_H
# define BIN_H

# include "./env.h"

# define PWD_HOME "HOME"

int	bin_echo(char **args, t_env **env);
int	bin_cd(char **args, t_env **env);
int	bin_pwd(char **args, t_env **env);
int	bin_exit(char **args, t_env **env);
int	bin_env(char **args, t_env **env);
int	bin_export(char **args, t_env **env);
int	bin_unset(char **args, t_env **env);

#endif
