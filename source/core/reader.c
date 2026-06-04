/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reader.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/31 00:00:00 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/03 00:00:00 by fcardozo        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../headers/core.h"
#include "../../headers/runner.h"
#include "../../headers/lexer.h"
#include "../../headers/parser.h"
#include "../../headers/sh_signal.h"
#include <readline/history.h>
#include <readline/readline.h>

#define SH_PROMPT "minishell$ "

static t_ast	*core_parse(char *line)
{
	t_manager	*manager;
	t_ast		*ast;

	manager = lexer(line);
	if (!manager)
		return (NULL);
	ast = parser_controller(manager);
	lexer_free(manager);
	return (ast);
}

static int	core_process_line(t_shell *shell, char *line)
{
	t_ast	*ast;

	ast = core_parse(line);
	if (!ast)
		return (rn_status_set(&shell->env, 2), 0);
	if (ast->error)
	{
		sh_stxerr(ast->error_type);
		rn_status_set(&shell->env, 2);
		parser_free_ast(ast);
		return (0);
	}
	rn_execute(ast->root, &shell->env);
	parser_free_ast(ast);
	return (0);
}

int	core_loop(t_shell *shell)
{
	char	*line;

	if (!shell)
		return (1);
	while (shell->running)
	{
		line = readline(SH_PROMPT);
		if (!line)
		{
			if (g_signal == SIGINT)
			{
				rn_status_set(&shell->env, 130);
				g_signal = 0;
				continue ;
			}
			write(STDOUT_FILENO, "exit\n", 5);
			return (rn_status_get(&shell->env));
		}
		if (g_signal == SIGINT)
			rn_status_set(&shell->env, 130);
		g_signal = 0;
		if (*line)
			add_history(line);
		if (core_process_line(shell, line))
			shell->running = FALSE;
		free(line);
	}
	return (rn_status_get(&shell->env));
}
