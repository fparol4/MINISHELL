/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/10 17:14:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/10 17:14:00 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_bin.h"

int	bin_pwd(char **args, t_env **env)
{
	char	buffer[4096];
	char	*fallback;

	(void)args;
	if (getcwd(buffer, sizeof(buffer)))
	{
		ft_putendl_fd(buffer, 1);
		return (0);
	}
	fallback = env_get(env, ENV_PWD);
	if (fallback)
	{
		ft_putendl_fd(fallback, 1);
		return (0);
	}
	perror("minishell: pwd");
	return (1);
}
