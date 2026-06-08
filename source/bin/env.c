/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:16 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:16 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_bin.h"

int	bin_env(char **args, t_env **env)
{
	t_env	*item;

	if (args && args[0])
		return (sh_err2("env", args[0], "No such file or directory"), 127);
	item = *env;
	while (item)
	{
		if (item->value && ft_strcmp(item->key, ENV_ERRCODE) != 0)
		{
			ft_putstr_fd(item->key, 1);
			ft_putchar_fd('=', 1);
			ft_putendl_fd(item->value, 1);
		}
		item = item->next;
	}
	return (0);
}
