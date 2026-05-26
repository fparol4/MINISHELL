/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:38 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:38 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

int	bin_exit(char **args, t_env **env)
{
	int	code;

	(void)env;
	ft_putendl_fd("exit", 1);
	if (!args || !args[0])
		exit(0);
	if (!ft_vatoi(args[0]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[0], 2);
		ft_putendl_fd(": numeric argument required", 2);
		env_set(env, ENV_ERRCODE, ft_itoa(2));
		exit(2);
	}
	if (args[1])
	{
		ft_putendl_fd("minishell: exit: too many arguments", 2);
		env_set(env, ENV_ERRCODE, ft_itoa(1));
		return (1);
	}
	code = ft_atoi(args[0]) & 0xFF;
	exit(code);
}
