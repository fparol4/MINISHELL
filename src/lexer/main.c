/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:08:05 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/25 19:05:01 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(void)
{
	char	*input;

	while (1)
	{
		input = readline("minishell ");
		if (!input)
			return (0);
		lexer_controll(&input);
		free(input);
	}
	return (0);
}
