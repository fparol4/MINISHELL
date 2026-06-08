#include "snow.h"
#include "../headers/parser.h"

static t_ast	*parse_input(const char *input)
{
	t_manager	*m;
	t_ast		*ast;

	m = lexer(input);
	if (!m)
		return (NULL);
	ast = parser_controller(m);
	lexer_free(m);
	return (ast);
}

describe(parser_simple_command)
{
	it("parses a single word")
	{
		t_ast		*ast;
		t_simple	*simple;

		ast = parse_input("ls");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		asserteq(ast->root->type, PNODE_CMD);
		simple = &ast->root->t_define.simple;
		asserteq_str(((char **)simple->args.items)[0], "ls");
		assert(((char **)simple->args.items)[1] == NULL);
		parser_free_ast(ast);
	}

	it("parses multiple arguments")
	{
		t_ast		*ast;
		t_simple	*simple;

		ast = parse_input("echo hello world");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		simple = &ast->root->t_define.simple;
		asserteq_str(((char **)simple->args.items)[0], "echo");
		asserteq_str(((char **)simple->args.items)[1], "hello");
		asserteq_str(((char **)simple->args.items)[2], "world");
		assert(((char **)simple->args.items)[3] == NULL);
		parser_free_ast(ast);
	}

	it("sets expand flag when dollar is in any arg")
	{
		t_ast		*ast;
		t_simple	*simple;

		ast = parse_input("echo $HOME");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(simple->expand, TRUE);
		parser_free_ast(ast);
	}
}

describe(parser_redirections)
{
	it("parses redirect out")
	{
		t_ast		*ast;
		t_simple	*simple;

		ast = parse_input("ls > out");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		simple = &ast->root->t_define.simple;
		asserteq(simple->redirs.length, 1);
		asserteq(((t_parser_redir *)simple->redirs.items)[0].type, REDIR_OUT);
		asserteq_str(((t_parser_redir *)simple->redirs.items)[0].file, "out");
		parser_free_ast(ast);
	}

	it("parses redirect in")
	{
		t_ast		*ast;
		t_simple	*simple;

		ast = parse_input("cat < file");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(((t_parser_redir *)simple->redirs.items)[0].type, REDIR_IN);
		asserteq_str(((t_parser_redir *)simple->redirs.items)[0].file, "file");
		parser_free_ast(ast);
	}

	it("parses append")
	{
		t_ast		*ast;
		t_simple	*simple;

		ast = parse_input("echo hi >> log");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(((t_parser_redir *)simple->redirs.items)[0].type,
			REDIR_APPEND);
		parser_free_ast(ast);
	}

	it("parses heredoc")
	{
		t_ast		*ast;
		t_simple	*simple;

		ast = parse_input("cat << EOF");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(((t_parser_redir *)simple->redirs.items)[0].type,
			REDIR_HEREDOC);
		asserteq_str(((t_parser_redir *)simple->redirs.items)[0].file, "EOF");
		parser_free_ast(ast);
	}

	it("preserves redir order")
	{
		t_ast		*ast;
		t_simple	*simple;

		ast = parse_input("cmd > o1 > o2");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(simple->redirs.length, 2);
		asserteq_str(((t_parser_redir *)simple->redirs.items)[0].file, "o1");
		asserteq_str(((t_parser_redir *)simple->redirs.items)[1].file, "o2");
		parser_free_ast(ast);
	}

	it("parses redir before args: < infile grep foo")
	{
		t_ast		*ast;
		t_simple	*simple;

		ast = parse_input("< infile grep foo");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(((t_parser_redir *)simple->redirs.items)[0].type, REDIR_IN);
		asserteq_str(((char **)simple->args.items)[0], "grep");
		parser_free_ast(ast);
	}
}

describe(parser_pipe)
{
	it("parses a simple pipe")
	{
		t_ast		*ast;
		t_command	*left;
		t_command	*right;

		ast = parse_input("ls | cat");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		asserteq(ast->root->type, PNODE_PIPE);
		left = ast->root->t_define.pipe.left;
		right = ast->root->t_define.pipe.right;
		asserteq(left->type, PNODE_CMD);
		asserteq(right->type, PNODE_CMD);
		asserteq_str(((char **)left->t_define.simple.args.items)[0], "ls");
		asserteq_str(((char **)right->t_define.simple.args.items)[0], "cat");
		parser_free_ast(ast);
	}

	it("builds a left-associative chain for three stages")
	{
		t_ast		*ast;
		t_command	*root;

		ast = parse_input("ls | grep foo | wc -l");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		root = ast->root;
		asserteq(root->type, PNODE_PIPE);
		asserteq(root->t_define.pipe.left->type, PNODE_PIPE);
		asserteq(root->t_define.pipe.right->type, PNODE_CMD);
		parser_free_ast(ast);
	}

	it("parses pipe with redirections on each side")
	{
		t_ast		*ast;
		t_command	*left;
		t_command	*right;

		ast = parse_input("ls -la | cat > out");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		left = ast->root->t_define.pipe.left;
		right = ast->root->t_define.pipe.right;
		asserteq(left->t_define.simple.redirs.length, 0);
		asserteq(right->t_define.simple.redirs.length, 1);
		parser_free_ast(ast);
	}
}

describe(parser_errors)
{
	it("returns error on pipe at start")
	{
		t_ast	*ast;

		ast = parse_input("| echo");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		asserteq(ast->error_type, SNTX_PIPE_START);
		parser_free_ast(ast);
	}

	it("returns error on pipe at end")
	{
		t_ast	*ast;

		ast = parse_input("echo |");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		asserteq(ast->error_type, SNTX_PIPE_END);
		parser_free_ast(ast);
	}

	it("returns error on redir without target")
	{
		t_ast	*ast;

		ast = parse_input("cat <");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		asserteq(ast->error_type, SNTX_REDIR_NO_TARGET);
		parser_free_ast(ast);
	}

	it("returns NULL on unclosed quote (caught by lexer)")
	{
		t_ast	*ast;

		ast = parse_input("echo 'hello");
		assert(ast == NULL);
	}

	it("returns NULL on empty input")
	{
		t_ast	*ast;

		ast = parse_input("");
		assert(ast == NULL);
	}
}

snow_main();
