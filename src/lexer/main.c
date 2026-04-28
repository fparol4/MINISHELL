/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:08:05 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/22 10:49:21 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	//t_manager	*manager_token;
	int				test;
	char			check;
	t_scanner		*sc;

	check = '\0';
	sc = malloc(sizeof(*sc));
	test = 3;
	while (test)
	{
		sc->input = readline("minishell ");
		if (!sc->input)
			return (0);
		scanner_init(sc, (const char *)sc->input);
		while (!scanner_is_end(sc))
		{
			check = scanner_current(sc);
			ft_printf("O valor é: %c\n", check);
			scanner_advance(sc);
		}
		lexer_controll(sc);
		free(sc->input);
		test--;
	}
	return (0);
}
