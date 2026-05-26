/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 14:18:59 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/25 14:18:59 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"
#include "./common.c"

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
