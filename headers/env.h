/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/04 18:57:11 by g-alves-          #+#    #+#             */
/*   Updated: 2026/06/04 19:55:25 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# define ENV_PWD "PWD"
# define ENV_OPWD "OLDPWD"
# define ENV_ERRCODE "$?"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

t_env	*env_init(char **env);
int		env_size(t_env **envm);
int		env_set(t_env **env, const char *key, const char *value);
int		env_unset(t_env **env, const char *key);
int		env_free(t_env **envm);
char	*env_get(t_env **env, const char *key);
char	**env_toarr(t_env **env);

#endif
