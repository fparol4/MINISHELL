/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 20:14:14 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 20:14:14 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MODULES_ENV_H
# define MODULES_ENV_H

# include <stdlib.h>
# include "../../../headers/env.h"
# include "../../../headers/errors.h"
# include "../../../headers/shared.h"

int		env_node_free(t_env *node);
int		env_free(t_env **env);
int		env_set(t_env **env, const char *key, const char *value);
int		env_size(t_env **envm);
int		env_unset(t_env **env, const char *key);
char	*env_get(t_env **env, const char *key);
char	**env_toarr(t_env **env);
t_env	*env_node_new(const char *key, const char *value);
t_env	*env_init(char **env);

#endif
