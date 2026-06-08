/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.h"

static t_manager	*lex(const char *input)
{
	return (lexer(input));
}

static t_list_token	*nth_token(t_manager *m, unsigned int n)
{
	t_node	*cur;

	cur = m->head;
	while (n-- && cur)
		cur = cur->next;
	if (!cur)
		return (NULL);
	return ((t_list_token *)cur->content);
}

static unsigned int	token_count(t_manager *m)
{
	t_node			*cur;
	unsigned int	n;

	n = 0;
	cur = m->head;
	while (cur)
	{
		n++;
		cur = cur->next;
	}
	return (n);
}


describe(lexer_words)
{
	{
	t_manager	*m;

	it("tokenizes a simple command into words")
	{
		m = lex("ls -la");
		assert(m != NULL);
		asserteq(token_count(m), 2);
		asserteq(nth_token(m, 0)->type, TOKEN_WORD);
		asserteq_str(nth_token(m, 0)->value, "ls");
		asserteq(nth_token(m, 1)->type, TOKEN_WORD);
		asserteq_str(nth_token(m, 1)->value, "-la");
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("keeps single-quoted content as one token")
	{
		m = lex("echo 'hello world'");
		assert(m != NULL);
		asserteq(token_count(m), 2);
		asserteq_str(nth_token(m, 1)->value, "'hello world'");
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("keeps mixed quotes in one word")
	{
		m = lex("echo 'one'\"two\"three");
		assert(m != NULL);
		asserteq(token_count(m), 2);
		asserteq_str(nth_token(m, 1)->value, "'one'\"two\"three");
		asserteq(nth_token(m, 1)->quoted, TRUE);
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("keeps quoted pipe characters inside words")
	{
		m = lex("echo '|' | cat");
		assert(m != NULL);
		asserteq(token_count(m), 4);
		asserteq_str(nth_token(m, 1)->value, "'|'");
		asserteq(nth_token(m, 1)->type, TOKEN_WORD);
		asserteq(nth_token(m, 2)->type, TOKEN_PIPE);
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("sets expand=true when dollar present")
	{
		m = lex("echo $HOME");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->expand, TRUE);
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("sets expand=false without dollar")
	{
		m = lex("echo hello");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->expand, FALSE);
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("returns empty list on whitespace-only input")
	{
		m = lex("   ");
		assert(m != NULL);
		asserteq(token_count(m), 0);
		lexer_free(m);
	}
	}
}

describe(lexer_operators)
{
	{
	t_manager	*m;

	it("tokenizes pipe")
	{
		m = lex("ls | cat");
		assert(m != NULL);
		asserteq(token_count(m), 3);
		asserteq(nth_token(m, 1)->type, TOKEN_PIPE);
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("tokenizes redirect in")
	{
		m = lex("cat < file");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->type, TOKEN_REDIR_IN);
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("tokenizes redirect out")
	{
		m = lex("ls > out");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->type, TOKEN_REDIR_OUT);
		asserteq_str(nth_token(m, 2)->value, "out");
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("tokenizes append")
	{
		m = lex("echo hi >> out");
		assert(m != NULL);
		asserteq(nth_token(m, 2)->type, TOKEN_APPEND);
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("tokenizes heredoc")
	{
		m = lex("cat << EOF");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->type, TOKEN_HEREDOC);
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("tokenizes a full pipeline with redirections")
	{
		m = lex("ls -la | cat > out");
		assert(m != NULL);
		asserteq(token_count(m), 6);
		asserteq(nth_token(m, 2)->type, TOKEN_PIPE);
		asserteq(nth_token(m, 5)->type, TOKEN_WORD);
		lexer_free(m);
	}
	}
	{
	t_manager	*m;

	it("marks consecutive pipe operators as invalid tokens")
	{
		m = lex("echo || cat");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->type, TOKEN_NONE);
		lexer_free(m);
	}
	}
}

describe(lexer_errors)
{
	{
	t_manager	*m;

	it("returns NULL on unclosed single quote")
	{
		m = lex("echo 'hello");
		assert(m == NULL);
	}
	}
	{
	t_manager	*m;

	it("returns NULL on unclosed double quote")
	{
		m = lex("echo \"hello");
		assert(m == NULL);
	}
	}
	{
	t_manager	*m;

	it("returns empty list on empty input")
	{
		m = lex("");
		assert(m != NULL);
		asserteq(token_count(m), 0);
		lexer_free(m);
	}
	}
}

snow_main();
