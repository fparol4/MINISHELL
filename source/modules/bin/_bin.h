/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bin.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:13:25 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:13:25 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BIN_H
# define BIN_H

# include "../../../headers/builtin.h"
# include "../../../headers/shared.h"

int	bin_cd(char **args, t_env **env);
int	bin_echo(char **args, t_env **env);
int	bin_env(char **args, t_env **env);
int	bin_exit(char **args, t_env **env);
int	bin_export(char **args, t_env **env);
int	bin_pwd(char **args, t_env **env);
int	bin_unset(char **args, t_env **env);

#endif
