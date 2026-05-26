/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/26 15:42:38 by fcardozo         #+#    #+#             */
/*   Updated: 2026/05/26 15:42:38 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../headers/minishell.h"

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
		ft_putendl_fd(fallback, 1);
	return (1);
}
