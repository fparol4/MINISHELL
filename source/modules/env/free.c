/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 14:06:40 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/31 14:06:40 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./env.h"

int	env_free(t_env **env)
{
	t_env	*node;
	t_env	*next;

	if (!env || !*env)
		return (1);
	node = *env;
	while (node)
	{
		next = node->next;
		f_freenode(node);
		node = next;
	}
	*env = NULL;
	return (1);
}
