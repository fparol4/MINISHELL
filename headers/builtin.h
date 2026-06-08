/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:54:39 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 18:56:03 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "env.h"

# define PWD_HOME "HOME"

int	bin_echo(char **args, t_env **env);
int	bin_cd(char **args, t_env **env);
int	bin_pwd(char **args, t_env **env);
int	bin_exit(char **args, t_env **env);
int	bin_env(char **args, t_env **env);
int	bin_export(char **args, t_env **env);
int	bin_unset(char **args, t_env **env);

#endif
