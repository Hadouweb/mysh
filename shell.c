#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include "lexer.h"
#include "parser.h"
#include "execute.h"
#include "command.h"

void	debug_print_lexer(lexer_t *lexerbuf)
{
	tok_t	*l;

	l = lexerbuf->llisttok;
	while (l)
	{
		printf("\033[33mdata: [%s] type: [%d]\033[0m\n", l->data, l->type);
		l = l->next;
	}

}

void	debug_print_ast_tree(ASTreeNode *node, int nb_tab, int side)
{
	if (node)
	{
		if (side == 0)
			printf("\033[33m%.*s LEFT [%s]\033[0m\n", nb_tab, "\t\t\t\t\t\t", node->szData);
		else if (side == 1)
			printf("\033[36m%.*s RIGH [%s]\033[0m\n", nb_tab, "\t\t\t\t\t\t", node->szData);
		else
			printf("\033[35m%.*s ROOT [%s]\033[0m\n", nb_tab, "\t\t\t\t\t\t", node->szData);

		debug_print_ast_tree(node->left, nb_tab + 1, 0);
		debug_print_ast_tree(node->right, nb_tab + 1, 1);
	}
}

int main()
{
	// ignore Ctrl-\ Ctrl-C Ctrl-Z signals
	ignore_signal_for_shell();

	// set the prompt
	set_prompt("swoorup % ");

	while (1)
	{
		char* linebuffer;
		size_t len;

		/*
		 *	struct tok {
		 *		char* 	data;
		 *		int 	type;
		 *		tok_t* 	next;
		 *	};
		 *
		 * 	struct lexer
		 *	{
		 *		tok_t* 	llisttok;
		 *		int 	ntoks;
		 *	};
		 */
		lexer_t lexerbuf;

		/*
		 *	typedef struct ASTreeNode
		 *	{
		 *		int type;
		 *		char* szData;
		 *		struct ASTreeNode* left;
		 *		struct ASTreeNode* right;
		 *
		 *	} ASTreeNode;
		 */
		ASTreeNode* exectree;

		// keep getline in a loop in case interruption occurs
		int again = 1;
		while (again) {
			again = 0;
			printf("%s", getprompt());
			linebuffer = NULL;
			len = 0;
			ssize_t nread = getline(&linebuffer, &len, stdin);
			if (nread <= 0 && errno == EINTR) {
				again = 1;        	// signal interruption, read again
				clearerr(stdin);	// clear the error
			}
		}

		// user pressed ctrl-D
		if (feof(stdin)) {
			exit(0);
			return 0;
		}

		// lexically analyze and build a list of tokens
		lexer_build(linebuffer, len, &lexerbuf);
		//debug_print_lexer(&lexerbuf);
		// parse the tokens into an abstract syntax tree
		if (!lexerbuf.ntoks || parse(&lexerbuf, &exectree) != 0) {
			continue;
		}
		debug_print_ast_tree(exectree, 0, -1);

		execute_syntax_tree(exectree);

		// free the structures
		ASTreeNodeDelete(exectree);
		lexer_destroy(&lexerbuf);
	}

	return 0;
}

