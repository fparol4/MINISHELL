/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:41 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:41 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_bin.h"

int	bin_unset(char **args, t_env **env)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (args[i])
	{
		if (!sh_isidentifier(args[i]))
		{
			ft_putstr_fd("minishell: unset: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putendl_fd("': not a valid identifier", 2);
			status = 1;
		}
		else
			env_unset(env, args[i]);
		i++;
	}
	return (status);
}
