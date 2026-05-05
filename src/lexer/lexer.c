/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: g-alves- <g-alves-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/04 22:09:36 by g-alves-          #+#    #+#             */
/*   Updated: 2026/05/04 20:25:40 by g-alves-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void			ft_state_machine(t_manager *manager,
						t_scanner *input, t_rules *rules);
static t_list_token	*get_word(t_manager *manager, t_scanner *input,
						t_rules *rules);
static t_list_token	*get_operator(t_manager *manager, t_scanner *input,
						t_rules *rules);
static t_token_type	define_type(char *type);
// static char			*ft_capture_word(char *input);
// static char			*get_redir_or_pipe(char *input);

t_manager	*lexer_controll(t_scanner	*input)
{
	t_manager		*manager;
	t_rules			*rules;

	manager = malloc(sizeof(t_manager));
	manager->head = NULL;
	manager->tail = NULL;
	rules = malloc(sizeof(t_rules));
	init_char_table(&rules->table);
	init_token_table(&rules->table);
	ft_state_machine(manager, input, rules);
	if (manager->head)
	{
		ft_printf("-----------THE NODE TOKEN HAVE THIS ELEMENTS-----------\n");
		ft_dlist_iter(manager->head, ft_print_list);
	}
	return (manager);
}

static void	ft_state_machine(t_manager *manager, t_scanner *input,
	t_rules *rules)
{
	unsigned int	props;

	if (!input)
		return ;
	while (!scanner_is_end(input))
	{
		props = 0;
		props = rules->table.props[scanner_current(input)];
		if (props & (P_ALPHA | P_DIGIT | P_DQUOTE | P_SQUOTE | P_SYMBOL))
			get_word(manager, input, rules);
		else if (props & P_SYMBOL)
			get_operator(manager, input, rules);
		else
			scanner_advance(input);
	}
}

static t_list_token	*get_word(t_manager *manager, t_scanner *input,
		t_rules *rules)
{
	unsigned int	state;
	unsigned int	props;
	char			*word;
	t_list_token	*token;

	state = P_NONE;
	scanner_mark_start(input);
	while (!scanner_is_end(input))
	{
		props = rules->table.props[scanner_current(input)];
		update_quote_state(props, &state);
		if ((state == P_NONE) && (props & (P_SPACE)))
			break ;
		scanner_advance(input);
	}
	word = scanner_extract(input);
	token = add_token_to_list(manager, word, TOKEN_WORD);
	free(word);
	return (token);
}

static t_list_token	*get_operator(t_manager *manager, t_scanner *input,
						t_rules *rules)
{
	unsigned int	state;
	unsigned int	props;
	char			*type;
	t_list_token	*token;

	state = P_NONE;
	scanner_mark_start(input);
	while (!scanner_is_end(input))
	{
		props = rules->table.props[scanner_current(input)];
		update_quote_state(props, &state);
		if ((state == P_NONE)
			&& (!(props & (L_PIPE | L_REDIR_IN | L_REDIR_OUT))))
			break ;
		scanner_advance(input);
	}
	type = scanner_extract(input);
	token = add_token_to_list(manager, type, define_type(type));
	free(type);
	return (token);
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

