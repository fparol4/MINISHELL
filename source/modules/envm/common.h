/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   common.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENVM_COMMON_H
# define ENVM_COMMON_H

# include "../../../headers/minishell.h"

t_env	*f_newnode(const char *key, const char *value);
int		f_freenode(t_env *node);

#endif
