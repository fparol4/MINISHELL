/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:09:36 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/07 17:50:54 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_manager	*lexer_controll(char	*input)
{
	t_manager	manager;

	add_token_to_list(&manager, input, NULL);
	ft_get_token(input);
}

char	*ft_get_token(char *input)
{
	ft_is_space(input);

}
