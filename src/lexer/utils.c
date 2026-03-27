/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/07 01:19:06 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/27 18:54:13 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list_token	*add_token_to_list(t_manager *manager, char *value,
	t_token_type type)
{
	t_node			*node;
	t_list_token	*token;

	token = malloc(sizeof(t_list_token));
	token->value = value;
	token->type = type;

	node = ft_create_node(token);
	ft_dlist_add_back(manager, node);
	return (token);
}

void	ft_print_list(void *content)
{
	t_list_token	*token;

	token = (t_list_token *)content;
	ft_printf("the type is: %i | ", token->type);
	ft_printf("the content is: %s\n", token->value);
}

char	*ft_tokenizer_quotes(char *input, char quote)
{
	char	*start;
	char	*word;

	start = NULL;
	if (quote == *input)
	{
		input++;
		start = input;
		while (*input && *input != quote)
			(*input)++;
		if (*input == quote)
		{
			word = malloc((input - start) * sizeof(char) + 1);
			if (!word)
				return (NULL);
			ft_memcpy(word, start, (input - start));
			word[input - start] = '\0';
			input++;
			return (word);
		}
		input++;
	}
	write(1, "\n\nINVALID_QUOTE\n\n", 18);
	return (NULL);
}

