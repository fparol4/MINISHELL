/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:08:05 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/26 19:20:31 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	//t_manager	*manager_token;
	char		*input;
	int			test;

	test = 3;
	while (test)
	{
		input = readline("minishell ");
		if (!input)
			return (0);
		lexer_controll(&input);
		free(input);
		test--;
	}
	return (0);
}
