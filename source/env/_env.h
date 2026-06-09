/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _env.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:56 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:56 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _ENV_H
# define _ENV_H

# include "../../headers/minishell.h"
# include <stdlib.h>

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
