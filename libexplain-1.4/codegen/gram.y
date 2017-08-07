/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2013 Peter Miller
 * Written by Peter Miller <pmiller@opensource.org.au>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

%{

#include <libexplain/ac/stdio.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>

#include <libexplain/sizeof.h>

#include <codegen/gram.h>
#include <codegen/node.h>

#define YYDEBUG 0
#define YYERROR_VERBOSE 1

%}

%token AUTO
%token CHAR
%token CONST
%token DOUBLE
%token ELLIPSIS
%token ENUM
%token EXTERN
%token FLOAT
%token IDENTIFIER
%token INT
%token JUNK
%token LONG
%token REGISTER
%token SHORT
%token SIGNED
%token STATIC
%token STRUCT
%token TYPEDEF
%token TYPE_NAME
%token UNION
%token UNSIGNED
%token VOID
%token VOLATILE

%union
{
    struct node_t *lv_node;
    const char    *lv_string;
}

%type <lv_string> IDENTIFIER
%type <lv_string> TYPE_NAME
%type <lv_node> declaration
%type <lv_node> declaration_specifiers
%type <lv_node> declarator
%type <lv_node> direct_declarator
%type <lv_node> enum_specifier
%type <lv_node> init_declarator
%type <lv_node> init_declarator_list
%type <lv_node> parameter_declaration
%type <lv_node> parameter_list
%type <lv_node> parameter_type_list
%type <lv_node> pointer
%type <lv_node> storage_class_specifier
%type <lv_node> struct_or_union
%type <lv_node> struct_or_union_specifier
%type <lv_node> type_qualifier
%type <lv_node> type_qualifier_list
%type <lv_node> type_specifier
%type <lv_node> optional_semicolon

%{


extern YYSTYPE yylval;
static const char *pos;


static void
lex_open(const char *text)
{
    pos = text;
}


static void
lex_close(void)
{
    pos = 0;
}


typedef struct res_tab_t res_tab_t;
struct res_tab_t
{
    const char *name;
    int value;
};

/*
 * Keep thid table in strcmp order
 * so that bsearch will work
 */
static const res_tab_t res_tab[] =
{
    { "...", ELLIPSIS },
    { "acl_t", TYPE_NAME },
    { "acl_type_t", TYPE_NAME },
    { "auto", AUTO },
    { "char", CHAR },
    { "class", JUNK }, // avoid so C++ doesn't barf
    { "const", CONST },
    { "dev_t", TYPE_NAME },
    { "double", DOUBLE },
    { "enum", ENUM },
    { "extern", EXTERN },
    { "fd_set", TYPE_NAME },
    { "float", FLOAT },
    { "fpos_t", TYPE_NAME },
    { "gid_t", TYPE_NAME },
    { "iconv_t", TYPE_NAME },
    { "int", INT },
    { "long", LONG },
    { "mode_t", TYPE_NAME },
    { "off_t", TYPE_NAME },
    { "operator", JUNK }, // avoid so C++ doesn't barf
    { "pid_t", TYPE_NAME },
    { "private", JUNK }, // avoid so C++ doesn't barf
    { "protected", JUNK }, // avoid so C++ doesn't barf
    { "ptrdiff_t", TYPE_NAME },
    { "public", JUNK }, // avoid so C++ doesn't barf
    { "register", REGISTER },
    { "short", SHORT },
    { "signed", SIGNED },
    { "sigset_t", TYPE_NAME },
    { "size_t", TYPE_NAME },
    { "socklen_t", TYPE_NAME },
    { "ssize_t", TYPE_NAME },
    { "static", STATIC },
    { "struct", STRUCT },
    { "template", JUNK }, // avoid so C++ doesn't barf
    { "time_t", TYPE_NAME },
    { "typedef", TYPEDEF },
    { "uid_t", TYPE_NAME },
    { "union", UNION },
    { "unsigned", UNSIGNED },
    { "useconds_t", TYPE_NAME },
    { "va_list", TYPE_NAME },
    { "void", VOID },
    { "volatile", VOLATILE },
};


static int
reserved(const char *s)
{
    const res_tab_t *tp;

    /* FIXME: use bearch */
    for (tp = res_tab; tp < ENDOF(res_tab); ++tp)
    {
        if (0 == strcmp(s, tp->name))
            return tp->value;
    }
    return IDENTIFIER;
}


static int
yylex(void)
{
    static char buf[256];
    for (;;)
    {
        char *bp = buf;
        unsigned char c;
        c = *pos++;
        switch (c)
        {
        case '\0':
            --pos;
            return 0;

        case ' ':
        case '\t':
        case '\n':
            break;

        default:
            return c;

        case '.':
            if (pos[0] == '.' && pos[1] == '.')
            {
                pos += 2;
                return ELLIPSIS;
            }
            return c;

        case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
        case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
        case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u':
        case 'v': case 'w': case 'x': case 'y': case 'z':
        case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
        case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
        case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
        case 'V': case 'W': case 'X': case 'Y': case 'Z':
        case '_':
            for (;;)
            {
                if (bp < buf + sizeof(buf) - 1)
                    *bp++ = c;
                c = *pos++;
                switch (c)
                {
                case '0': case '1': case '2': case '3': case '4':
                case '5': case '6': case '7': case '8': case '9':
                case 'a': case 'b': case 'c': case 'd': case 'e': case 'f':
                case 'g': case 'h': case 'i': case 'j': case 'k': case 'l':
                case 'm': case 'n': case 'o': case 'p': case 'q': case 'r':
                case 's': case 't': case 'u': case 'v': case 'w': case 'x':
                case 'y': case 'z':
                case 'A': case 'B': case 'C': case 'D': case 'E': case 'F':
                case 'G': case 'H': case 'I': case 'J': case 'K': case 'L':
                case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R':
                case 'S': case 'T': case 'U': case 'V': case 'W': case 'X':
                case 'Y': case 'Z':
                case '_':
                    continue;

                default:
                    --pos;
                    break;
                }
                break;
            }
            *bp = '\0';
            yylval.lv_string = buf;
            return reserved(buf);
        }
    }
}


static void
yyerror(const char *s)
{
    fprintf(stderr, "parsing declaration: %s\n", s);
    exit(EXIT_FAILURE);
}


// Different versions of bison and yacc emit code chunks and function
// prototypes in different orders.  We need this prototype declaration
// because the call to yyparse, below, would complain in the cases where
// there is not yet a yyparse prototype in scope.
int yyparse(void);

static node_t *result;
#if YYDEBUG
extern int yydebug;
#endif


node_t *
grammar(const char *text)
{
#if YYDEBUG
    yydebug = 1;
#endif
    lex_open(text);
    yyparse();
    lex_close();
    return result;
}

%}

%%

success
    : declaration
        {
            result = $1;
        }
    ;

declaration
    : declaration_specifiers optional_semicolon
        {
            $$ = node_new("declaration");
            node_push_back($$, $1);
            node_push_back($$, $2);
        }
    | declaration_specifiers init_declarator_list optional_semicolon
        {
            $$ = node_new("declaration");
            node_push_back($$, $1);
            node_push_back($$, $2);
            node_push_back($$, $3);
        }
    ;

optional_semicolon
    : /* empty */
        { $$ = node_new_literal(";"); }
    | ';'
        { $$ = node_new_literal(";"); }
    ;

declaration_specifiers
    : storage_class_specifier
        {
            $$ = node_new("declaration_specifiers");
            node_push_back($$, $1);
        }
    | storage_class_specifier declaration_specifiers
        {
            $$ = node_new("declaration_specifiers");
            node_push_back($$, $1);
            node_push_back($$, $2);
        }
    | type_specifier
        {
            $$ = node_new("declaration_specifiers");
            node_push_back($$, $1);
        }
    | type_specifier declaration_specifiers
        {
            $$ = node_new("declaration_specifiers");
            node_push_back($$, $1);
            node_push_back($$, $2);
        }
    | type_qualifier
        {
            $$ = node_new("declaration_specifiers");
            node_push_back($$, $1);
        }
    | type_qualifier declaration_specifiers
        {
            $$ = node_new("declaration_specifiers");
            node_push_back($$, $1);
            node_push_back($$, $2);
        }
    ;

init_declarator_list
    : init_declarator
        {
            $$ = node_new("init_declarator_list");
            node_push_back($$, $1);
        }
    | init_declarator_list ',' init_declarator
        {
            $$ = $1;
            node_push_back($$, node_new_literal(","));
            node_push_back($$, $3);
        }
    ;

init_declarator
    : declarator
        {
            $$ = node_new("init_declarator");
            node_push_back($$, $1);
        }
    ;

storage_class_specifier
    : TYPEDEF
        {
            $$ = node_new("init_declarator");
            node_push_back($$, node_new_literal("typedef"));
        }
    | EXTERN
        {
            $$ = node_new("init_declarator");
            node_push_back($$, node_new_literal("extern"));
        }
    | STATIC
        {
            $$ = node_new("init_declarator");
            node_push_back($$, node_new_literal("static"));
        }
    | AUTO
        {
            $$ = node_new("init_declarator");
            node_push_back($$, node_new_literal("auto"));
        }
    | REGISTER
        {
            $$ = node_new("init_declarator");
            node_push_back($$, node_new_literal("register"));
        }
    ;

type_specifier
    : VOID
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal("void"));
        }
    | CHAR
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal("char"));
        }
    | SHORT
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal("short"));
        }
    | INT
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal("int"));
        }
    | LONG
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal("long"));
        }
    | FLOAT
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal("float"));
        }
    | DOUBLE
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal("double"));
        }
    | SIGNED
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal("signed"));
        }
    | UNSIGNED
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal("unsigned"));
        }
    | struct_or_union_specifier
        {
            $$ = node_new("type_specifier");
            node_push_back($$, $1);
        }
    | enum_specifier
        {
            $$ = node_new("type_specifier");
            node_push_back($$, $1);
        }
    | TYPE_NAME
        {
            $$ = node_new("type_specifier");
            node_push_back($$, node_new_literal($1));
        }
    ;

struct_or_union_specifier
    : struct_or_union IDENTIFIER
        {
            $$ = node_new("struct_or_union_specifier");
            node_push_back($$, $1);
            node_push_back($$, node_new_literal($2));
        }
    ;

struct_or_union
    : STRUCT
        {
            $$ = node_new("struct_or_union");
            node_push_back($$, node_new_literal("struct"));
        }
    | UNION
        {
            $$ = node_new("struct_or_union");
            node_push_back($$, node_new_literal("union"));
        }
    ;

enum_specifier
    : ENUM IDENTIFIER
        {
            $$ = node_new("enum_specifier");
            node_push_back($$, node_new_literal("enum"));
            node_push_back($$, node_new_literal($2));
        }
    ;

type_qualifier
    : CONST
        {
            $$ = node_new("type_qualifier");
            node_push_back($$, node_new_literal("const"));
        }
    | VOLATILE
        {
            $$ = node_new("type_qualifier");
            node_push_back($$, node_new_literal("volatile"));
        }
    ;

declarator
    : pointer direct_declarator
        {
            $$ = node_new("declarator");
            node_push_back($$, $1);
            node_push_back($$, $2);
        }
    | direct_declarator
        {
            $$ = node_new("declarator");
            node_push_back($$, $1);
        }
    ;

direct_declarator
    : IDENTIFIER
        {
            $$ = node_new("direct_declarator");
            node_push_back($$, node_new_literal($1));
        }
    | '(' declarator ')'
        {
            $$ = node_new("direct_declarator");
            node_push_back($$, node_new_literal("("));
            node_push_back($$, $2);
            node_push_back($$, node_new_literal(")"));
        }
    | direct_declarator '[' ']'
        {
            $$ = node_new("direct_declarator");
            node_push_back($$, $1);
            node_push_back($$, node_new_literal("["));
            node_push_back($$, node_new_literal("]"));
        }
    | direct_declarator '(' parameter_type_list ')'
        {
            $$ = node_new("direct_declarator");
            node_push_back($$, $1);
            node_push_back($$, node_new_literal("("));
            node_push_back($$, $3);
            node_push_back($$, node_new_literal(")"));
        }
    | direct_declarator '(' ')'
        {
            $$ = node_new("direct_declarator");
            node_push_back($$, $1);
            node_push_back($$, node_new_literal("("));
            node_push_back($$, node_new_literal(")"));
        }
    ;

pointer
    : '*'
        {
            $$ = node_new("pointer");
            node_push_back($$, node_new_literal("*"));
        }
    | '*' type_qualifier_list
        {
            $$ = node_new("pointer");
            node_push_back($$, node_new_literal("*"));
            node_push_back($$, $2);
        }
    | '*' pointer
        {
            $$ = node_new("pointer");
            node_push_back($$, node_new_literal("*"));
            node_push_back($$, $2);
        }
    | '*' type_qualifier_list pointer
        {
            $$ = node_new("pointer");
            node_push_back($$, node_new_literal("*"));
            node_push_back($$, $2);
            node_push_back($$, $3);
        }
    ;

type_qualifier_list
    : type_qualifier
        {
            $$ = node_new("type_qualifier_list");
            node_push_back($$, $1);
        }
    | type_qualifier_list type_qualifier
        {
            $$ = $1;
            node_push_back($$, $2);
        }
    ;

parameter_type_list
    : VOID
        {
            $$ = node_new("parameter_type_list");
            node_push_back($$, node_new_literal("void"));
        }
    | parameter_list
        {
            $$ = node_new("parameter_type_list");
            node_push_back($$, $1);
        }
    | parameter_list ',' ELLIPSIS
        {
            $$ = node_new("parameter_type_list");
            node_push_back($$, node_new_literal(","));
            node_push_back($$, node_new_literal("..."));
        }
    ;

parameter_list
    : parameter_declaration
        {
            $$ = node_new("parameter_list");
            node_push_back($$, $1);
        }
    | parameter_list ',' parameter_declaration
        {
            $$ = $1;
            node_push_back($$, node_new_literal(","));
            node_push_back($$, $3);
        }
    ;

parameter_declaration
    : declaration_specifiers declarator
        {
            $$ = node_new("parameter_declaration");
            node_push_back($$, $1);
            node_push_back($$, $2);
        }
    ;


// vim: set ts=8 sw=4 et :
