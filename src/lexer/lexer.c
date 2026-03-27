/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:09:36 by g-alves-          #+#    #+#             */
/*   Updated: 2026/03/27 16:40:42 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void			ft_get_token(t_manager *manager,
						char *input);
static char			*ft_capture_word(char **input);
static char			*get_redir_or_pipe(char **input);
static t_token_type	define_type(char *type);

t_manager	*lexer_controll(char	**input)
{
	t_manager		*manager;
	char			*tmp;

	manager = malloc(sizeof(t_manager));
	manager->head = NULL;
	manager->tail = NULL;
	tmp = *input;
	ft_get_token(manager, tmp);
	if (manager->head)
	{
		ft_printf("-----------THE NODE TOKEN HAVE THIS ELEMENTS-----------\n");
		ft_dlist_iter(manager->head, ft_print_list);
	}
	return (manager);
}

static void	ft_get_token(t_manager *manager, char *input)
{
	char	*word;
	char	*type;

	word = NULL;
	type = NULL;
	if (!input)
		return ;
	while (*input)
	{
		if (ft_jump_space(&input))
			return ;
		word = ft_capture_word(&input);
		if (word)
			add_token_to_list(manager, word, TOKEN_WORD);
		if (ft_jump_space(&input))
			return ;
		type = get_redir_or_pipe(&input);
		if (type)
			add_token_to_list(manager, type, define_type(type));
		if (ft_jump_space(&input))
			return ;
	}
	free(word);
	free(type);
}

static char	*ft_capture_word(char **input)
{
	char	*word;
	char	*first_position;

	if (!input || !*input || !**input || **input == ' ' || **input == '|'
		|| **input == '<' || **input == '>')
		return (NULL);
	first_position = *input;
	if (**input == '"' || **input == '\'')
	{
		word = ft_tokenizer_quotes(input, **input);
		return (word);
	}
	while (**input && (**input != ' ' && **input != '|' && **input != '<'
			&& **input != '>'))
		(*input)++;
	word = malloc((*input - first_position) * sizeof(char) + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, first_position, (*input - first_position));
	word[*input - first_position] = '\0';
	return (word);
}

static char	*get_redir_or_pipe(char **input)
{
	char		*type;
	char		*first_position;
	size_t		len;

	if (!input || !*input || !**input)
		return (NULL);
	first_position = *input;
	len = 0;
	if ((**input == '<' && *(*input + 1) == '<')
		|| (**input == '>' && *(*input + 1) == '>'))
		len = 2;
	if ((**input == ' ' || **input == '|' || **input == '<' || **input == '>')
		&& len == 0)
		len = 1;
	if (len == 0)
		return (NULL);
	type = malloc(len * sizeof(char) + 1);
	if (!type)
		return (NULL);
	ft_memcpy(type, first_position, len);
	type[len] = '\0';
	(*input) += len;
	return (type);
}

static t_token_type	define_type(char *type)
{
	if (ft_strncmp(type, ">>", 2) == 0)
		return (TOKEN_APPEND);
	if (ft_strncmp(type, "<<", 2) == 0)
		return (TOKEN_HEREDOC);
	if (ft_strncmp(type, "|", 1) == 0)
		return (TOKEN_PIPE);
	if (ft_strncmp(type, "<", 1) == 0)
		return (TOKEN_REDIR_IN);
	if (ft_strncmp(type, ">", 1) == 0)
		return (TOKEN_REDIR_OUT);
	return (0);
}
