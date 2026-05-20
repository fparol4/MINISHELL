/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/19 18:03:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/19 18:03:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	bin_exit(char **args, t_env **env)
{
	int	code;

	(void)env;
	ft_putstr_fd("exit", 1);
	if (!args[0])
		exit(0);
	if (!ft_vatoi(args[0]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putendl_fd(": numeric argument required\n", 2);
		env_set(env, ENV_ERRCODE, ft_itoa(2));
		exit(2);
	}
	if (args[2])
	{
		ft_putendl_fd("minishell: exit: too many arguments\n", 2);
		env_set(env, ENV_ERRCODE, ft_itoa(1));
		return (1);
	}
	code = ft_atoi(args[1]) & 0xFF;
	exit(code);
}
