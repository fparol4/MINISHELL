/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/25 10:42:39 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/25 10:42:39 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	bin_env(char **args, t_env **env)
{
	t_env	*item;

	(void)args;
	item = *env;
	while (item)
	{
		if (item->value)
		{
			ft_putstr_fd(item->key, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(item->value, 1);
		}
		item = item->next;
	}
	return (0);
}
