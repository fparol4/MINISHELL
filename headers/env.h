/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:17 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:17 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_H
# define ENV_H

# define ENV_PWD "PWD"
# define ENV_OPWD "OPWD"
# define ENV_ERRCODE "$?"

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

t_env				*env_init(char **env);
int					env_size(t_env **envm);
int					env_set(t_env **env, const char *key, const char *value);
int					env_unset(t_env **env, const char *key);
int					env_free(t_env **envm);
char				*env_get(t_env **env, const char *key);
char				**env_toarr(t_env **env);

#endif
