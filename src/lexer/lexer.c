/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:09:36 by g-alves-          #+#    #+#             */
/*   Updated: 2026/04/28 19:29:35 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void			ft_state_machine(t_manager *manager,
						t_scanner *input, t_char_table table);
static void			get_word(t_manager *manager, t_scanner *input,
						t_char_table table, unsigned int props);
// static char			*ft_capture_word(char *input);
// static char			*get_redir_or_pipe(char *input);
// static t_token_type	define_type(char *type);

t_manager	*lexer_controll(t_scanner	*input)
{
	t_manager		*manager;
	t_char_table	table;
	char			*tmp;

	manager = malloc(sizeof(t_manager));
	manager->head = NULL;
	manager->tail = NULL;
	init_char_table(table);
	tmp = input;
	ft_state_machine(manager, tmp, table);
	if (manager->head)
	{
		ft_printf("-----------THE NODE TOKEN HAVE THIS ELEMENTS-----------\n");
		ft_dlist_iter(manager->head, ft_print_list);
	}
	return (manager);
}

static void	ft_state_machine(t_manager *manager, t_scanner *input,
	t_char_table table)
{
	unsigned int	props;

	if (!input)
		return ;
	while (!scanner_is_end(input))
	{
		props = table.props[scanner_current(input)];
		if (props & (P_ALPHA | P_DIGIT))
			get_word(manager, input, table, props);
		else if (props & P_SYMBOL)
			get_operator(manager, input, table);
		else
			scanner_advance(input);
	}
}

static void	get_word(t_manager *manager, t_scanner *input, t_char_table table,
	unsigned int props)
{
	unsigned int	state;
	unsigned int	props;

	state = P_NONE;
	scanner_mark_start(input);
	while (!scanner_is_end(input))
	{
		props = table.props[scanner_current(input)];
		if (props & P_SQUOTE)
			state = P_SQUOTE;
		else if (props & P_DQUOTE)
			state = P_DQUOTE;
		else if (!(state & (P_SQUOTE | P_DQUOTE)))
		{
			scanner_extract(input);
		}
	}
}

// static char	*ft_capture_word(char *input)
// {
// 	char	*word;
// 	char	*first_position;
// 	char	quote;

// 	quote = '\0';
// 	word = NULL;
// 	if (scanner_current(input) == P_)
// 		return (NULL);
// 	if (!input !*input || *input == ' ' || *input == '|'
// 		|| **input == '<' || **input == '>')
// 		return (NULL);
// 	first_position = *input;
// 	while (**input && ((**input != ' ' && **input != '|' && **input != '<'
// 				&& **input != '>') || quote))
// 		quote = is_quote(input, quote);
// 	word = malloc((*input - first_position) * sizeof(char) + 1);
// 	if (!word)
// 		return (NULL);
// 	ft_memcpy(word, first_position, (*input - first_position));
// 	word[*input - first_position] = '\0';
// 	return (word);
// }

// static char	*get_redir_or_pipe(char *input)
// {
// 	char		*type;
// 	char		*first_position;
// 	size_t		len;

// 	if (!input || !*input || !**input)
// 		return (NULL);
// 	if (ft_jump_space(input))
// 		return (NULL);
// 	first_position = *input;
// 	len = 0;
// 	if ((*input == '<' && *input == '<')
// 		|| (*input == '>' && *(*input + 1) == '>'))
// 		len = 2;
// 	if ((*input == ' ' || **input == '|' || **input == '<' || *input == '>')
// 		&& len == 0)
// 		len = 1;
// 	if (len == 0)
// 		return (NULL);
// 	type = malloc(len * sizeof(char) + 1);
// 	if (!type)
// 		return (NULL);
// 	ft_memcpy(type, first_position, len);
// 	type[len] = '\0';
// 	(*input) += len;
// 	return (type);
// }

// static t_token_type	define_type(char *type)
// {
// 	if (ft_strncmp(type, ">>", 2) == 0)
// 		return (TOKEN_APPEND);
// 	if (ft_strncmp(type, "<<", 2) == 0)
// 		return (TOKEN_HEREDOC);
// 	if (ft_strncmp(type, "|", 1) == 0)
// 		return (TOKEN_PIPE);
// 	if (ft_strncmp(type, "<", 1) == 0)
// 		return (TOKEN_REDIR_IN);
// 	if (ft_strncmp(type, ">", 1) == 0)
// 		return (TOKEN_REDIR_OUT);
// 	return (0);
// }
