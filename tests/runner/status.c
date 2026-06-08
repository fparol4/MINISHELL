/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   status.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:23:50 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:23:50 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../tester.h"

describe(rn_status)
{
		t_env *env;

	it("sets and gets the shell status")
	{
		env = NULL;
		asserteq(rn_status_set(&env, 42), 0);
		asserteq(rn_status_get(&env), 42);
		env_free(&env);
	}
}
