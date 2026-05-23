/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_test.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/11 11:43:50 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/21 19:59:42 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	run_test(char *input)
{
	t_manager	*manager_test__;
	t_scanner	scanner__;

	scanner_init(&scanner__, input);

	manager_test__ = lexer_controll(&scanner__);

	if (manager_test__->head)
	{
		ft_printf("\n|---------THE NODE TOKEN HAVE THIS ELEMENTS---------|\n");
		ft_dlist_iter(manager_test__->head, ft_print_list);
	}

	free(manager_test__);
}

int	main(void)
{
	run_test("ls -la");
	run_test("ls | cat");
	run_test("ls -la | \"$var\"");
	run_test("echo hel'lo world");
	run_test("ls | cat > out | echo hel'lo worl'd tes\"tando algo 123 \" algo");
}

void	ft_print_list(void *content)
{
	t_list_token	*token;

	token = (t_list_token *)content;
	ft_printf("the type is: %i | ", token->type);
	ft_printf("the content is: %s\n", token->value);
}
