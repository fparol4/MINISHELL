/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/09 10:21:58 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/09 10:21:58 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "_runner.h"

int	rn_status_set(t_env **env, int status)
{
	char	*value;
	int		ret;

	value = ft_itoa(status);
	if (!value)
		return (1);
	ret = env_set(env, ENV_ERRCODE, value);
	free(value);
	return (ret);
}

int	rn_status_get(t_env **env)
{
	char	*value;

	value = env_get(env, ENV_ERRCODE);
	if (!value)
		return (0);
	return (ft_atoi(value));
}
