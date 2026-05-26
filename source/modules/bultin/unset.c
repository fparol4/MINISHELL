/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:38 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:38 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	bin_unset(char **args, t_env **env)
{
	int	i;

	i = 0;
	while (args[i])
		env_unset(env, args[i++]);
	return (0);
}
