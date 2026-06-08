/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcardozo <fcardozo@student.42.org.br>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/08 19:25:13 by fcardozo         #+#    #+#             */
/*   Updated: 2026/06/08 19:25:13 by fcardozo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "tester.h"

#define TEST_SHARED_PARSER
#include "shared.c"

describe(parser_simple_command)
{
	t_ast		*ast;
	t_simple	*simple;
	t_ast		*ast;
	t_simple	*simple;
	t_ast		*ast;
	t_simple	*simple;
	t_ast		*ast;
	t_simple	*simple;
	t_ast		*ast;
	t_simple	*simple;

	it("parses a single word")
	{
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
		ast = parse_input("echo $HOME");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(simple->expand, TRUE);
		parser_free_ast(ast);
	}
	it("parses mixed single and double quotes in one word")
	{
		ast = parse_input("echo 'one'\"two\"three");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		simple = &ast->root->t_define.simple;
		asserteq_str(((char **)simple->args.items)[1], "'one'\"two\"three");
		assert(((char **)simple->args.items)[2] == NULL);
		parser_free_ast(ast);
	}
	it("parses empty quoted command args")
	{
		ast = parse_input("echo \"\"");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		simple = &ast->root->t_define.simple;
		asserteq_str(((char **)simple->args.items)[1], "\"\"");
		assert(((char **)simple->args.items)[2] == NULL);
		parser_free_ast(ast);
	}
}

describe(parser_redirections)
{
	t_ast			*ast;
	t_simple		*simple;
	t_ast			*ast;
	t_simple		*simple;
	t_ast			*ast;
	t_simple		*simple;
	t_ast			*ast;
	t_simple		*simple;
	t_ast			*ast;
	t_simple		*simple;
	t_parser_redir	*redir;
	t_ast			*ast;
	t_simple		*simple;
	t_parser_redir	*redir;
	t_ast			*ast;
	t_simple		*simple;
	t_ast			*ast;
	t_simple		*simple;
	t_ast			*ast;
	t_simple		*simple;
	t_ast			*ast;
	t_simple		*simple;
	t_parser_redir	*redir;

	it("parses redirect out")
	{
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
		ast = parse_input("cat < file");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(((t_parser_redir *)simple->redirs.items)[0].type, REDIR_IN);
		asserteq_str(((t_parser_redir *)simple->redirs.items)[0].file, "file");
		parser_free_ast(ast);
	}
	it("parses append")
	{
		ast = parse_input("echo hi >> log");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(((t_parser_redir *)simple->redirs.items)[0].type,
			REDIR_APPEND);
		parser_free_ast(ast);
	}
	it("parses heredoc")
	{
		ast = parse_input("cat << EOF");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(((t_parser_redir *)simple->redirs.items)[0].type,
			REDIR_HEREDOC);
		asserteq_str(((t_parser_redir *)simple->redirs.items)[0].file, "EOF");
		parser_free_ast(ast);
	}
	it("preserves quoted metadata on redirection targets")
	{
		ast = parse_input("cat > \"two words\"");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		simple = &ast->root->t_define.simple;
		redir = (t_parser_redir *)simple->redirs.items;
		asserteq_str(redir[0].file, "\"two words\"");
		asserteq(redir[0].quoted, TRUE);
		asserteq(redir[0].expand, FALSE);
		parser_free_ast(ast);
	}
	it("disables heredoc expansion for quoted delimiters")
	{
		ast = parse_input("cat << 'EOF'");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		simple = &ast->root->t_define.simple;
		redir = (t_parser_redir *)simple->redirs.items;
		asserteq_str(redir[0].file, "'EOF'");
		asserteq(redir[0].quoted, TRUE);
		asserteq(redir[0].expand, FALSE);
		parser_free_ast(ast);
	}
	it("preserves redir order")
	{
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
		ast = parse_input("< infile grep foo");
		assert(ast != NULL);
		simple = &ast->root->t_define.simple;
		asserteq(((t_parser_redir *)simple->redirs.items)[0].type, REDIR_IN);
		asserteq_str(((char **)simple->args.items)[0], "grep");
		parser_free_ast(ast);
	}
	it("parses redirections without command words")
	{
		ast = parse_input("> file");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		simple = &ast->root->t_define.simple;
		asserteq(simple->args.length, 0);
		asserteq(simple->redirs.length, 1);
		asserteq_str(((t_parser_redir *)simple->redirs.items)[0].file, "file");
		parser_free_ast(ast);
	}
	it("parses multiple redirections before and after args")
	{
		ast = parse_input("< in grep foo > out >> log");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		simple = &ast->root->t_define.simple;
		redir = (t_parser_redir *)simple->redirs.items;
		asserteq(simple->redirs.length, 3);
		asserteq(redir[0].type, REDIR_IN);
		asserteq(redir[1].type, REDIR_OUT);
		asserteq(redir[2].type, REDIR_APPEND);
		asserteq_str(((char **)simple->args.items)[0], "grep");
		asserteq_str(((char **)simple->args.items)[1], "foo");
		parser_free_ast(ast);
	}
}

describe(parser_pipe)
{
	t_ast		*ast;
	t_command	*left;
	t_command	*right;
	t_ast		*ast;
	t_command	*root;
	t_ast		*ast;
	t_command	*left;
	t_command	*right;
	t_ast		*ast;
	t_command	*left;
	t_command	*right;

	it("parses a simple pipe")
	{
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
		ast = parse_input("ls -la | cat > out");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		left = ast->root->t_define.pipe.left;
		right = ast->root->t_define.pipe.right;
		asserteq(left->t_define.simple.redirs.length, 0);
		asserteq(right->t_define.simple.redirs.length, 1);
		parser_free_ast(ast);
	}
	it("parses pipes with surrounding whitespace and quoted pipes")
	{
		ast = parse_input("  echo '|'   |   cat  ");
		assert(ast != NULL);
		asserteq(ast->error, 0);
		asserteq(ast->root->type, PNODE_PIPE);
		left = ast->root->t_define.pipe.left;
		right = ast->root->t_define.pipe.right;
		asserteq_str(((char **)left->t_define.simple.args.items)[1], "'|'");
		asserteq_str(((char **)right->t_define.simple.args.items)[0], "cat");
		parser_free_ast(ast);
	}
}

describe(parser_errors)
{
	t_ast	*ast;
	t_ast	*ast;
	t_ast	*ast;
	t_ast	*ast;
	t_ast	*ast;
	t_ast	*ast;
	t_ast	*ast;
	t_ast	*ast;
	t_ast	*ast;

	it("returns error on pipe at start")
	{
		ast = parse_input("| echo");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		asserteq(ast->error_type, SNTX_PIPE_START);
		parser_free_ast(ast);
	}
	it("returns error on pipe at end")
	{
		ast = parse_input("echo |");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		asserteq(ast->error_type, SNTX_PIPE_END);
		parser_free_ast(ast);
	}
	it("returns error on redir without target")
	{
		ast = parse_input("cat <");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		asserteq(ast->error_type, SNTX_REDIR_NO_TARGET);
		parser_free_ast(ast);
	}
	it("returns error on consecutive operators")
	{
		ast = parse_input("echo || cat");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		parser_free_ast(ast);
	}
	it("returns error on invalid repeated output operators")
	{
		ast = parse_input("echo >>> file");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		parser_free_ast(ast);
	}
	it("returns error on adjacent heredoc and input operators")
	{
		ast = parse_input("cat << < file");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		parser_free_ast(ast);
	}
	it("returns error on redir followed by pipe")
	{
		ast = parse_input("echo > | cat");
		assert(ast != NULL);
		asserteq(ast->error, 1);
		parser_free_ast(ast);
	}
	it("returns NULL on unclosed quote (caught by lexer)")
	{
		ast = parse_input("echo 'hello");
		assert(ast == NULL);
	}
	it("returns NULL on empty input")
	{
		ast = parse_input("");
		assert(ast == NULL);
	}
}

snow_main();
