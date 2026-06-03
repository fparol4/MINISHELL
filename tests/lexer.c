#include "snow.h"
#include "../headers/lexer.h"
#include "../headers/scanner.h"

static t_manager	*lex(const char *input)
{
	t_scanner	sc;

	scanner_init(&sc, input);
	return (lexer(&sc));
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
	it("tokenizes a simple command into words")
	{
		t_manager	*m;

		m = lex("ls -la");
		assert(m != NULL);
		asserteq(token_count(m), 2);
		asserteq(nth_token(m, 0)->type, TOKEN_WORD);
		asserteq_str(nth_token(m, 0)->value, "ls");
		asserteq(nth_token(m, 1)->type, TOKEN_WORD);
		asserteq_str(nth_token(m, 1)->value, "-la");
		lexer_free(m);
	}

	it("keeps single-quoted content as one token")
	{
		t_manager	*m;

		m = lex("echo 'hello world'");
		assert(m != NULL);
		asserteq(token_count(m), 2);
		asserteq_str(nth_token(m, 1)->value, "'hello world'");
		lexer_free(m);
	}

	it("sets expand=true when dollar present")
	{
		t_manager	*m;

		m = lex("echo $HOME");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->expand, TRUE);
		lexer_free(m);
	}

	it("sets expand=false without dollar")
	{
		t_manager	*m;

		m = lex("echo hello");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->expand, FALSE);
		lexer_free(m);
	}

	it("returns empty list on whitespace-only input")
	{
		t_manager	*m;

		m = lex("   ");
		assert(m != NULL);
		asserteq(token_count(m), 0);
		lexer_free(m);
	}
}

describe(lexer_operators)
{
	it("tokenizes pipe")
	{
		t_manager	*m;

		m = lex("ls | cat");
		assert(m != NULL);
		asserteq(token_count(m), 3);
		asserteq(nth_token(m, 1)->type, TOKEN_PIPE);
		lexer_free(m);
	}

	it("tokenizes redirect in")
	{
		t_manager	*m;

		m = lex("cat < file");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->type, TOKEN_REDIR_IN);
		lexer_free(m);
	}

	it("tokenizes redirect out")
	{
		t_manager	*m;

		m = lex("ls > out");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->type, TOKEN_REDIR_OUT);
		asserteq_str(nth_token(m, 2)->value, "out");
		lexer_free(m);
	}

	it("tokenizes append")
	{
		t_manager	*m;

		m = lex("echo hi >> out");
		assert(m != NULL);
		asserteq(nth_token(m, 2)->type, TOKEN_APPEND);
		lexer_free(m);
	}

	it("tokenizes heredoc")
	{
		t_manager	*m;

		m = lex("cat << EOF");
		assert(m != NULL);
		asserteq(nth_token(m, 1)->type, TOKEN_HEREDOC);
		lexer_free(m);
	}

	it("tokenizes a full pipeline with redirections")
	{
		t_manager	*m;

		m = lex("ls -la | cat > out");
		assert(m != NULL);
		asserteq(token_count(m), 6);
		asserteq(nth_token(m, 2)->type, TOKEN_PIPE);
		asserteq(nth_token(m, 5)->type, TOKEN_WORD);
		lexer_free(m);
	}
}

describe(lexer_errors)
{
	it("returns NULL on unclosed single quote")
	{
		t_manager	*m;

		m = lex("echo 'hello");
		assert(m == NULL);
	}

	it("returns NULL on unclosed double quote")
	{
		t_manager	*m;

		m = lex("echo \"hello");
		assert(m == NULL);
	}

	it("returns empty list on empty input")
	{
		t_manager	*m;

		m = lex("");
		assert(m != NULL);
		asserteq(token_count(m), 0);
		lexer_free(m);
	}
}

snow_main();
