/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008-2010, 2012, 2013 Peter Miller
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

#include <libexplain/ac/assert.h>
#include <libexplain/ac/ctype.h>
#include <libexplain/ac/stdlib.h>
#include <libexplain/ac/string.h>

#include <libexplain/malloc.h>
#include <libexplain/output.h>
#include <libexplain/sizeof.h>
#include <libexplain/strdup.h>

#include <codegen/elastic_buffer.h>
#include <codegen/node.h>


node_t *
node_new(const char *name)
{
    node_t          *np;

    np = explain_malloc_or_die(sizeof(node_t));
    np->reference_count = 1;
    np->name = explain_strdup_or_die(name);
    np->child = 0;
    np->nchild = 0;
    np->nchild_max = 0;
    np->literal = 0;
    return np;
}


node_t *
node_copy(node_t *np)
{
    assert(np);
    assert(np->reference_count >= 1);
    np->reference_count++;
    return np;
}


void
node_free(node_t *np)
{
#if 0
    (void)np;
#else
    assert(np);
    assert(np->reference_count >= 1);
    np->reference_count--;
    if (np->reference_count <= 0)
    {
        size_t          j;

        for (j = 0; j < np->nchild; ++j)
            node_free(np->child[j]);
        if (np->child)
            free(np->child);
        np->child = 0;
        np->nchild = 0;
        np->nchild_max = 0;
        free((char *)(np->name));
        np->name = 0;
        if (np->literal)
            free((char *)(np->literal));
        np->literal = 0;
        free(np);
    }
#endif
}


node_t *
node_new_literal(const char *text)
{
    node_t          *np;

    np = node_new("LITERAL");
    np->literal = explain_strdup_or_die(text);
    return np;
}


int
node_is_a_literal(const node_t *np)
{
    return (np && np->name && 0 == strcmp(np->name, "LITERAL"));
}


void
node_push_back(node_t *parent, node_t *child)
{
    assert(parent);
    assert(parent->reference_count >= 1);
    assert(!node_is_a_literal(parent));
    assert(child);
    assert(child->reference_count >= 1);
    if (parent->nchild >= parent->nchild_max)
    {
        size_t          new_nchild_max;
        size_t          nbytes;
        node_t          **new_child;
        size_t          j;

        new_nchild_max = parent->nchild_max * 2 + 4;
        nbytes = sizeof(parent->child[0]) * new_nchild_max;
        new_child = explain_malloc_or_die(nbytes);
        for (j = 0; j < parent->nchild; ++j)
            new_child[j] = parent->child[j];
        if (parent->child)
            free(parent->child);
        parent->child = new_child;
        parent->nchild_max = new_nchild_max;
    }
    parent->child[parent->nchild++] = child;
}


static int
node_is_empty(const node_t *np)
{
    size_t          j;

    if (!np)
        return 1;
    if (node_is_a_literal(np))
    {
        const char *cp = np->literal;
        return (*cp == '\0');
    }
    for (j = 0; j < np->nchild; ++j)
    {
        node_t *cp = np->child[j];
        if (!node_is_empty(cp))
            return 0;
    }
    return 1;
}


static int
node_first_char(const node_t *np)
{
    size_t          j;

    if (!np)
        return 0;
    if (node_is_a_literal(np))
    {
        const char *cp = np->literal;
        return *cp;
    }
    for (j = 0; j < np->nchild; ++j)
    {
        node_t *cp = np->child[j];
        if (!node_is_empty(cp))
            return node_first_char(cp);
    }
    return 0;
}


static int
node_last_char(const node_t *np)
{
    size_t          j;

    if (!np)
        return 0;
    if (node_is_a_literal(np))
    {
        const char *cp = np->literal;
        size_t len = strlen(cp);
        return (len ? cp[len - 1] : 0);
    }
    for (j = 0; j < np->nchild; ++j)
    {
        size_t jj = np->nchild - 1 - j;
        node_t *cp = np->child[jj];
        if (!node_is_empty(cp))
            return node_last_char(cp);
    }
    return 0;
}


static int
isid(int c)
{
    return (isalnum((unsigned char)c) || c == '_');
}


static int
bad_pair(int c1, int c2)
{
    static const char *table[] =
    {
        "!=",
        "%=",
        "&&",
        "&=",
        "*=",
        "++",
        "+=",
        "--",
        "-=",
        "->",
        "..",
        "/*",
        "//",
        "/=",
        "<<",
        "<=",
        "==",
        ">=",
        ">>",
        "^=",
        "|=",
        "||",
        "~=",
        "??",
    };
    const char **cpp;

    for (cpp = table; cpp < ENDOF(table); ++cpp)
    {
        const char *cp = *cpp;
        if (c1 == cp[0] && c2 == cp[1])
            return 1;
    }
    if (isdigit((unsigned char)c1) && c2 == '.')
        return 1;
    if (c1 == '.' && isdigit((unsigned char)c2))
        return 1;
    return 0;
}


static int
is_declaration_specifiers(const node_t *np)
{
    return node_is(np, "declaration_specifiers");
}


static int
is_declarator(const node_t *np)
{
    return
        (
            node_is(np, "declarator")
        ||
            node_is(np, "init_declarator")
        ||
            node_is(np, "init_declarator_list")
        );
}


static int
need_space_between(const node_t *n1, const node_t *n2)
{
    unsigned char c1 = node_last_char(n1);
    unsigned char c2 = node_first_char(n2);
    if (isid(c1) && isid(c2))
        return 1;
    if (bad_pair(c1, c2))
        return 1;
    if (c1 == ',')
        return 1;
    if (is_declaration_specifiers(n1) && is_declarator(n2))
        return 1;
    return 0;
}


static int
need_tab_between(const node_t *n1, const node_t *n2)
{
    return (is_declaration_specifiers(n1) && is_declarator(n2));
}


void
node_print(const node_t *np, FILE *fp)
{
    if (node_is_a_literal(np))
    {
        assert(!np->nchild);
        fputs(np->literal, fp);
    }
    else
    {
        size_t          j;

        assert(!np->literal);
        for (j = 0; j < np->nchild; ++j)
        {
            if (j && need_space_between(np->child[j - 1], np->child[j]))
                putc(' ', fp);
            node_print(np->child[j], fp);
        }
    }
}


void
node_print_sb(const node_t *np, elastic_buffer_t *sb, node_print_style_t style)
{
    if
    (
        style == node_print_style_function
    &&
        np->nchild == 1
    &&
        node_is(np, "direct_declarator")
    &&
        node_is_a_literal(np->child[0])
    )
    {
        /* only wrapper_hang honors this. */
        elastic_buffer_putc(sb, '\n');
    }
    if (node_is(np, "parameter_type_list"))
        style = node_print_style_normal;

    if (node_is_a_literal(np))
    {
        assert(!np->nchild);
        elastic_buffer_puts(sb, np->literal);
    }
    else
    {
        size_t          j;

        assert(!np->literal);
        for (j = 0; j < np->nchild; ++j)
        {
            if (j)
            {
                if
                (
                    style == node_print_style_column16
                &&
                    need_tab_between(np->child[j - 1], np->child[j])
                )
                {
                    /* only wrapper_hang honors this */
                    elastic_buffer_putc(sb, '\t');
                }
                else if (need_space_between(np->child[j - 1], np->child[j]))
                {
                    elastic_buffer_putc(sb, ' ');
                }
            }
            node_print_sb(np->child[j], sb, style);
        }
    }
}


static void
indent2(FILE *fp, int depth)
{
    while (depth > 0)
    {
        fputs("  ", fp);
        --depth;
    }
}


static void
node_print_lisp_indented(const node_t *np, FILE *fp, int indent)
{
    if (node_is_a_literal(np))
    {
        assert(!np->nchild);
        indent2(fp, indent);
        fputc('"', fp);
        fputs(np->literal, fp);
        fputs("\"\n", fp);
    }
    else
    {
        size_t          j;

        assert(!np->literal);
        indent2(fp, indent);
        putc('(', fp);
        fputs(np->name, fp);
        if (np->nchild > 0)
        {
            fputc('\n', fp);
            for (j = 0; j < np->nchild; ++j)
            {
                node_print_lisp_indented(np->child[j], fp, indent + 1);
            }
            indent2(fp, indent);
        }
        fputs(")\n", fp);
    }
}


void
node_print_lisp(const node_t *np, FILE *fp)
{
    node_print_lisp_indented(np, fp, 0);
}


int
node_is(const node_t *np, const char *name)
{
    assert(name);
    return (0 == strcmp(np->name, name));
}


int
node_is_literal(const node_t *np, const char *text)
{
    if (!node_is_a_literal(np))
        return 0;
    assert(np->literal);
    assert(text);
    return (0 == strcmp(text, np->literal));
}


static node_t *
node_synth_type_specifier_void(void)
{
    node_t *np = node_new("type_specifier");
    node_push_back(np, node_new_literal("void"));
    return np;
}


static node_t *node_synth_or_die_declarator(node_t *np, int return_void);


static node_t *
node_synth_or_die_direct_declarator(node_t *np, int return_void)
{
    node_t *np2 = node_new("direct_declarator");
    assert(node_is(np, "direct_declarator"));

    /*
     * (direct_declarator "name")
     */
    if (np->nchild == 1 && node_is_a_literal(np->child[0]))
    {
        /* re-write the name */
        char buf[1000];
        snprintf(buf, sizeof(buf), "explain_%s_or_die", np->child[0]->literal);
        node_push_back(np2, node_new_literal(buf));
        return np2;
    }

    /*
     * (direct_declarator "(" declarator ")" )
     */
    if
    (
        np->nchild == 3
    &&
        node_is_literal(np->child[0], "(")
    &&
        node_is_literal(np->child[2], ")")
    )
    {
        node_push_back(np2, node_copy(np->child[0]));
        node_push_back
        (
            np2,
            node_synth_or_die_declarator(np->child[1], return_void)
        );
        node_push_back(np2, node_copy(np->child[2]));
        return np2;
    }

    /*
     * (direct_declarator (direct_declarator ...) "[" "]" )
     */
    if
    (
        np->nchild == 3
    &&
        node_is_literal(np->child[1], "[")
    &&
        node_is_literal(np->child[2], "]")
    )
    {
        node_push_back
        (
            np2,
            node_synth_or_die_direct_declarator(np->child[0], return_void)
        );
        node_push_back(np2, node_copy(np->child[1]));
        node_push_back(np2, node_copy(np->child[2]));
        return np2;
    }

    /*
     * (direct_delcarator (direct_declarator ...) "("
     *     (parameter_type_list ...) ")" )
     */
    if
    (
        np->nchild == 4
    &&
        node_is_literal(np->child[1], "(")
    &&
        node_is_literal(np->child[3], ")")
    )
    {
        node_push_back
        (
            np2,
            node_synth_or_die_direct_declarator(np->child[0], return_void)
        );
        node_push_back(np2, node_copy(np->child[1]));
        node_push_back(np2, node_copy(np->child[2]));
        node_push_back(np2, node_copy(np->child[3]));
        return np2;
    }

    /*
     * (direct_declarator (direct_declarator ...) "(" ")" )
     */
    if
    (
        np->nchild == 3
    &&
        node_is_literal(np->child[1], "(")
    &&
        node_is_literal(np->child[2], ")")
    )
    {
        node_push_back
        (
            np2,
            node_synth_or_die_direct_declarator(np->child[0], return_void)
        );
        node_push_back(np2, node_copy(np->child[1]));
        node_push_back(np2, node_copy(np->child[2]));
        return np2;
    }

    assert(!"should not reach here");
    return np2;
}


static node_t *
node_synth_or_die_declarator(node_t *np, int return_void)
{
    node_t *np2 = node_new("declarator");
    assert(node_is(np, "declarator"));
    switch (np->nchild)
    {
    default:
        assert(!"wong number of children");
        break;

    case 1:
        /* (declarator (direct_declarator ...)) */
        node_push_back
        (
            np2,
            node_synth_or_die_direct_declarator(np->child[0], return_void)
        );
        break;

    case 2:
        /* (declarator (pointer ...) (direct_declarator ...)) */
        if (!return_void)
            node_push_back(np2, node_copy(np->child[0]));
        node_push_back
        (
            np2,
            node_synth_or_die_direct_declarator(np->child[1], return_void)
        );
        break;
    }
    return np2;
}


static node_t *
node_synth_or_die_init_declarator(node_t *np, int return_void)
{
    node_t *np2 = node_new("init_declarator");
    assert(node_is(np, "init_declarator"));
    assert(np->nchild >= 1);
    node_push_back
    (
        np2,
        node_synth_or_die_declarator(np->child[0], return_void)
    );
    return np2;
}


static node_t *
node_synth_or_die_init_declarator_list(node_t *np, int return_void)
{
    node_t *np2 = node_new("init_declarator_list");
    assert(node_is(np, "init_declarator_list"));
    assert(np->nchild >= 1);
    node_push_back
    (
        np2,
        node_synth_or_die_init_declarator(np->child[0], return_void)
    );
    return np2;
}


static node_t *
node_synth_or_die_declaration(node_t *np, int return_void)
{
    node_t *np2 = node_new("declaration");
    assert(node_is(np, "declaration"));
    assert(np->nchild >= 2);
    if (return_void)
        node_push_back(np2, node_synth_type_specifier_void());
    else
        node_push_back(np2, node_copy(np->child[0]));
    node_push_back
    (
        np2,
        node_synth_or_die_init_declarator_list(np->child[1], return_void)
    );
    return np2;
}


static int
declaration_specifiers_is_int(const node_t *np)
{
    assert(node_is(np, "declaration_specifiers"));
    if (np->nchild != 1)
        return 0;
    np = np->child[0];
    return
        (
            node_is(np, "type_specifier")
        &&
            np->nchild == 1
        &&
            node_is_literal(np->child[0], "int")
        );
}


static int
direct_declarator_is_name(const node_t *np)
{
    assert(node_is(np, "direct_declarator"));
    return (np->nchild == 1 && node_is_a_literal(np->child[0]));
}


static int
direct_declarator_is_simple_function(const node_t *np)
{
    assert(node_is(np, "direct_declarator"));

    /*
     * (direct_declarator
     *   (direct_declarator "name")
     *   "("
     *   (parameter_type_list ...)
     *   ")"
     * )
     */
    if
    (
        np->nchild == 4
    &&
        direct_declarator_is_name(np->child[0])
    &&
        node_is_literal(np->child[1], "(")
    &&
        node_is_literal(np->child[3], ")")
    )
        return 1;

    /*
     * (direct_declarator
     *   (direct_declarator "name")
     *   "("
     *   ")"
     * )
     */
    if
    (
        np->nchild == 3
    &&
        direct_declarator_is_name(np->child[0])
    &&
        node_is_literal(np->child[1], "(")
    &&
        node_is_literal(np->child[2], ")")
    )
        return 1;

    /*
     * These are not the nodes you are looking for, move along.
     */
    return 0;
}


static int
declarator_is_simple_function(const node_t *np)
{
    assert(node_is(np, "declarator"));

    /*
     * (declarator (direct_declarator ...))
     * (declarator (pointer ...) (direct_declarator ...))
     */
    return
        (
            np->nchild == 1
        &&
            direct_declarator_is_simple_function(np->child[0])
        );
}


static int
init_declarator_is_simple_function(const node_t *np)
{
    assert(node_is(np, "init_declarator"));
    assert(np->nchild >= 1);
    return declarator_is_simple_function(np->child[0]);
}


static int
init_declarator_list0_is_simple_function(const node_t *np)
{
    assert(node_is(np, "init_declarator_list"));
    assert(np->nchild >= 1);
    return init_declarator_is_simple_function(np->child[0]);
}


int
function_returns_int(const node_t *np)
{
    assert(node_is(np, "declaration"));
    assert(np->nchild == 2 || np->nchild == 3);
    return
        (
            declaration_specifiers_is_int(np->child[0])
        &&
            init_declarator_list0_is_simple_function(np->child[1])
        );
}


static int
declarator_function_returns_pointer(const node_t *np)
{
    assert(node_is(np, "declarator"));

    /*
     * (declarator (direct_declarator ...))
     * (declarator (pointer ...) (direct_declarator ...))
     */
    return
        (
            np->nchild == 2
        &&
            direct_declarator_is_simple_function(np->child[1])
        );
}


static int
init_declarator_function_returns_pointer(const node_t *np)
{
    assert(node_is(np, "init_declarator"));
    assert(np->nchild >= 1);
    return declarator_function_returns_pointer(np->child[0]);
}


static int
init_declarator_list0_function_returns_pointer(const node_t *np)
{
    assert(node_is(np, "init_declarator_list"));
    assert(np->nchild >= 1);
    return init_declarator_function_returns_pointer(np->child[0]);
}


int
function_returns_pointer(const node_t *np)
{
    assert(node_is(np, "declaration"));
    assert(np->nchild == 2 || np->nchild == 3);
    return init_declarator_list0_function_returns_pointer(np->child[1]);
}


node_t *
node_synth_or_die(node_t *np, int use_return_value)
{
    assert(node_is(np, "declaration"));
    if (!function_returns_int(np))
        use_return_value = 1;
    return node_synth_or_die_declaration(np, !use_return_value);
}


static node_t *node_synth_on_error_declarator(node_t *np);


static node_t *
node_synth_on_error_direct_declarator(node_t *np)
{
    node_t *np2 = node_new("direct_declarator");
    assert(node_is(np, "direct_declarator"));

    /*
     * (direct_declarator "name")
     */
    if (np->nchild == 1 && node_is_a_literal(np->child[0]))
    {
        /* re-write the name */
        char bu[1000];
        snprintf(bu, sizeof(bu), "explain_%s_on_error", np->child[0]->literal);
        node_push_back(np2, node_new_literal(bu));
        return np2;
    }

    /*
     * (direct_declarator "(" declarator ")" )
     */
    if
    (
        np->nchild == 3
    &&
        node_is_literal(np->child[0], "(")
    &&
        node_is_literal(np->child[2], ")")
    )
    {
        node_push_back(np2, node_copy(np->child[0]));
        node_push_back(np2, node_synth_on_error_declarator(np->child[1]));
        node_push_back(np2, node_copy(np->child[2]));
        return np2;
    }

    /*
     * (direct_declarator (direct_declarator ...) "[" "]" )
     */
    if
    (
        np->nchild == 3
    &&
        node_is_literal(np->child[1], "[")
    &&
        node_is_literal(np->child[2], "]")
    )
    {
        node_push_back
        (
            np2,
            node_synth_on_error_direct_declarator(np->child[0])
        );
        node_push_back(np2, node_copy(np->child[1]));
        node_push_back(np2, node_copy(np->child[2]));
        return np2;
    }

    /*
     * (direct_delcarator (direct_declarator ...) "("
     *     (parameter_type_list ...) ")" )
     */
    if
    (
        np->nchild == 4
    &&
        node_is_literal(np->child[1], "(")
    &&
        node_is_literal(np->child[3], ")")
    )
    {
        node_push_back
        (
            np2,
            node_synth_on_error_direct_declarator(np->child[0])
        );
        node_push_back(np2, node_copy(np->child[1]));
        node_push_back(np2, node_copy(np->child[2]));
        node_push_back(np2, node_copy(np->child[3]));
        return np2;
    }

    /*
     * (direct_declarator (direct_declarator ...) "(" ")" )
     */
    if
    (
        np->nchild == 3
    &&
        node_is_literal(np->child[1], "(")
    &&
        node_is_literal(np->child[2], ")")
    )
    {
        node_push_back
        (
            np2,
            node_synth_on_error_direct_declarator(np->child[0])
        );
        node_push_back(np2, node_copy(np->child[1]));
        node_push_back(np2, node_copy(np->child[2]));
        return np2;
    }

    assert(!"should not reach here");
    return np2;
}


static node_t *
node_synth_on_error_declarator(node_t *np)
{
    node_t *np2 = node_new("declarator");
    assert(node_is(np, "declarator"));
    switch (np->nchild)
    {
    default:
        assert(!"wong number of children");
        break;

    case 1:
        /* (declarator (direct_declarator ...)) */
        node_push_back
        (
            np2,
            node_synth_on_error_direct_declarator(np->child[0])
        );
        break;

    case 2:
        /* (declarator (pointer ...) (direct_declarator ...)) */
        node_push_back(np2, node_copy(np->child[0]));
        node_push_back
        (
            np2,
            node_synth_on_error_direct_declarator(np->child[1])
        );
        break;
    }
    return np2;
}


static node_t *
node_synth_on_error_init_declarator(node_t *np)
{
    node_t *np2 = node_new("init_declarator");
    assert(node_is(np, "init_declarator"));
    assert(np->nchild >= 1);
    node_push_back(np2, node_synth_on_error_declarator(np->child[0]));
    return np2;
}


static node_t *
node_synth_on_error_init_declarator_list(node_t *np)
{
    node_t *np2 = node_new("init_declarator_list");
    assert(node_is(np, "init_declarator_list"));
    assert(np->nchild >= 1);
    node_push_back(np2, node_synth_on_error_init_declarator(np->child[0]));
    return np2;
}


static node_t *
node_synth_on_error_declaration(node_t *np)
{
    node_t *np2 = node_new("declaration");
    assert(node_is(np, "declaration"));
    assert(np->nchild >= 2);
    node_push_back(np2, node_copy(np->child[0]));
    node_push_back(np2, node_synth_on_error_init_declarator_list(np->child[1]));
    return np2;
}


node_t *
node_synth_on_error(node_t *np)
{
    assert(node_is(np, "declaration"));
    return node_synth_on_error_declaration(np);
}


static node_t *node_synth_result_variable_declarator(node_t *np);


static node_t *
node_synth_result_variable_direct_declarator(node_t *np)
{
    assert(node_is(np, "direct_declarator"));

    /*
     * (direct_declarator "name")
     */
    if (np->nchild == 1 && node_is_a_literal(np->child[0]))
    {
        /* re-write the name */
        node_t *np2 = node_new("direct_declarator");
        node_push_back(np2, node_new_literal("result"));
        return np2;
    }

    /*
     * (direct_declarator "(" declarator ")" )
     */
    if
    (
        np->nchild == 3
    &&
        node_is_literal(np->child[0], "(")
    &&
        node_is_literal(np->child[2], ")")
    )
    {
        node_t *np2 = node_new("direct_declarator");
        node_push_back(np2, node_copy(np->child[0]));
        node_push_back
        (
            np2,
            node_synth_result_variable_declarator(np->child[1])
        );
        node_push_back(np2, node_copy(np->child[2]));
        return np2;
    }

    /*
     * (direct_declarator (direct_declarator ...) "[" "]" )
     */
    if
    (
        np->nchild == 3
    &&
        node_is_literal(np->child[1], "[")
    &&
        node_is_literal(np->child[2], "]")
    )
    {
        node_t *np2 = node_new("direct_declarator");
        node_push_back
        (
            np2,
            node_synth_result_variable_direct_declarator(np->child[0])
        );
        node_push_back(np2, node_copy(np->child[1]));
        node_push_back(np2, node_copy(np->child[2]));
        return np2;
    }

    /*
     * (direct_delcarator (direct_declarator ...) "("
     *     (parameter_type_list ...) ")" )
     */
    if
    (
        np->nchild == 4
    &&
        node_is_literal(np->child[1], "(")
    &&
        node_is_literal(np->child[3], ")")
    )
    {
        return node_synth_result_variable_direct_declarator(np->child[0]);
    }

    /*
     * (direct_declarator (direct_declarator ...) "(" ")" )
     */
    if
    (
        np->nchild == 3
    &&
        node_is_literal(np->child[1], "(")
    &&
        node_is_literal(np->child[2], ")")
    )
    {
        return node_synth_result_variable_direct_declarator(np->child[0]);
    }

    assert(!"should not reach here");
    return 0;
}


static node_t *
node_synth_result_variable_declarator(node_t *np)
{
    node_t *np2 = node_new("declarator");
    assert(node_is(np, "declarator"));
    switch (np->nchild)
    {
    default:
        assert(!"wong number of children");
        break;

    case 1:
        /* (declarator (direct_declarator ...)) */
        node_push_back
        (
            np2,
            node_synth_result_variable_direct_declarator(np->child[0])
        );
        break;

    case 2:
        /* (declarator (pointer ...) (direct_declarator ...)) */
        node_push_back(np2, node_copy(np->child[0]));
        node_push_back
        (
            np2,
            node_synth_result_variable_direct_declarator(np->child[1])
        );
        break;
    }
    return np2;
}


static node_t *
node_synth_result_variable_init_declarator(node_t *np)
{
    node_t *np2 = node_new("init_declarator");
    assert(node_is(np, "init_declarator"));
    assert(np->nchild >= 1);
    node_push_back(np2, node_synth_result_variable_declarator(np->child[0]));
    return np2;
}


static node_t *
node_synth_result_variable_init_declarator_list(node_t *np)
{
    node_t *np2 = node_new("init_declarator_list");
    assert(node_is(np, "init_declarator_list"));
    assert(np->nchild >= 1);
    node_push_back
    (
        np2,
        node_synth_result_variable_init_declarator(np->child[0])
    );
    return np2;
}


static node_t *
node_synth_result_variable_declaration(node_t *np)
{
    node_t *np2 = node_new("declaration");
    assert(node_is(np, "declaration"));
    assert(np->nchild >= 2);
    node_push_back(np2, node_copy(np->child[0]));
    node_push_back
    (
        np2,
        node_synth_result_variable_init_declarator_list(np->child[1])
    );
    return np2;
}


node_t *
node_synth_result_variable(node_t *np)
{
    assert(node_is(np, "declaration"));
    return node_synth_result_variable_declaration(np);
}


#define IS_AUTO      (1 <<  0)
#define IS_CHAR      (1 <<  1)
#define IS_CONST     (1 <<  2)
#define IS_DOUBLE    (1 <<  3)
#define IS_ENUM      (1 <<  4)
#define IS_EXTERN    (1 <<  5)
#define IS_FLOAT     (1 <<  6)
#define IS_INT       (1 <<  7)
#define IS_INTEGER   (1 <<  8) /* but not guaranteed to print with %d */
#define IS_LONG      (1 <<  9)
#define IS_LONGLONG  (1 << 10)
#define IS_REGISTER  (1 << 11)
#define IS_SHORT     (1 << 12)
#define IS_SIGNED    (1 << 13)
#define IS_SIZE_T    (1 << 14)
#define IS_SSIZE_T   (1 << 15)
#define IS_STATIC    (1 << 16)
#define IS_STRUCT    (1 << 17)
#define IS_TYPEDEF   (1 << 18)
#define IS_UNSIGNED  (1 << 19)
#define IS_VOID      (1 << 20)
#define IS_VOLATILE  (1 << 21)
#define IS_OFF_T     (1 << 22)
#define IS_SOCKLEN_T (1 << 23)
#define IS_PTRDIFF_T (1 << 24)
#define IS_TIME_T    (1 << 25)
#define IS_DEV_T     (1 << 26)
#define IS_UID_T     (1 << 27)
#define IS_GID_T     (1 << 28)


static int
declspec_add(int lhs, int rhs)
{
    int result = lhs | rhs;
    if ((lhs & IS_LONG) && (rhs & IS_LONG))
        result = (result & ~IS_LONG) | IS_LONGLONG;
    return result;
}


static int
grope_storage_class_specifier(node_t *np)
{
    assert(node_is(np, "storage_class_specifier"));
    assert(np->nchild == 1);
    assert(node_is_a_literal(np->child[0]));
    np = np->child[0];
    if (node_is_literal(np, "typedef"))
        return IS_TYPEDEF;
    if (node_is_literal(np, "extern"))
        return IS_EXTERN;
    if (node_is_literal(np, "static"))
        return IS_STATIC;
    if (node_is_literal(np, "auto"))
        return IS_AUTO;
    if (node_is_literal(np, "register"))
        return IS_REGISTER;
    assert(!"grammar says this can't happen");
    return 0;
}


static int
grope_type_specifier(node_t *np)
{
    assert(node_is(np, "type_specifier"));
    assert(np->nchild == 1);
    np = np->child[0];
    if (node_is_literal(np, "char"))
        return IS_CHAR;
    if (node_is_literal(np, "short"))
        return IS_SHORT;
    if (node_is_literal(np, "int"))
        return IS_INT;
    if (node_is_literal(np, "long"))
        return IS_LONG;
    if (node_is_literal(np, "float"))
        return IS_FLOAT;
    if (node_is_literal(np, "double"))
        return IS_DOUBLE;
    if (node_is_literal(np, "signed"))
        return IS_SIGNED;
    if (node_is_literal(np, "unsigned"))
        return IS_UNSIGNED;
    if (node_is(np, "struct_or_union_specifier"))
        return IS_STRUCT;
    if (node_is(np, "enum_specifier"))
        return IS_ENUM;
    if (node_is(np, "enum_specifier"))
        return IS_ENUM;

    /* everything else is a 'type_secifier = TYPE_NAME' production */
    assert(node_is_a_literal(np));
    if (node_is_literal(np, "acl_t") )
        return IS_STRUCT;
    if (node_is_literal(np, "dev_t"))
        return IS_DEV_T;
    if (node_is_literal(np, "DIR"))
        return IS_STRUCT;
    if (node_is_literal(np, "fd_set"))
        return IS_STRUCT;
    if (node_is_literal(np, "FILE") )
        return IS_STRUCT;
    if (node_is_literal(np, "gid_t"))
        return IS_GID_T;
    if (node_is_literal(np, "iconv_t") )
        return IS_STRUCT;
    if (node_is_literal(np, "mode_t") )
        return IS_INTEGER;
    if (node_is_literal(np, "off_t"))
        return IS_OFF_T;
    if (node_is_literal(np, "pid_t") )
        return IS_INTEGER;
    if (node_is_literal(np, "ptrdiff_t") )
        return IS_PTRDIFF_T;
    if (node_is_literal(np, "sigset_t"))
        return IS_INTEGER;
    if (node_is_literal(np, "size_t"))
        return IS_SIZE_T;
    if (node_is_literal(np, "socklen_t") )
        return IS_SOCKLEN_T;
    if (node_is_literal(np, "ssize_t"))
        return IS_SSIZE_T;
    if (node_is_literal(np, "time_t") )
        return IS_TIME_T;
    if (node_is_literal(np, "uid_t") )
        return IS_UID_T;
    return 0;
}


static int
grope_type_qualifier(node_t *np)
{
    assert(node_is(np, "type_qualifier"));
    assert(np->nchild == 1);
    np = np->child[0];
    if (node_is_literal(np, "const"))
        return IS_CONST;
    if (node_is_literal(np, "volatile"))
        return IS_VOLATILE;
    assert(!"grammer says this can't happen");
    return 0;
}


static int
grope_declspec(node_t *np)
{
    int result = 0;
    for (;;)
    {
        assert(node_is(np, "declaration_specifiers"));
        if (node_is(np->child[0], "storage_class_specifier"))
        {
            int tmp = grope_storage_class_specifier(np->child[0]);
            result = declspec_add(result, tmp);
        }
        else if (node_is(np->child[0], "type_specifier"))
        {
            int tmp = grope_type_specifier(np->child[0]);
            result = declspec_add(result, tmp);
        }
        else if (node_is(np->child[0], "type_qualifier"))
        {
            int tmp = grope_type_qualifier(np->child[0]);
            result = declspec_add(result, tmp);
        }
        else
            assert(!"the grammar says this can't happen");
        if (np->nchild < 2)
            return result;
        np = np->child[1];
    }
}


static int
direct_declarator_is_simple_name(node_t *np)
{
    assert(node_is(np, "direct_declarator"));
    return (np->nchild == 1 && node_is_a_literal(np->child[0]));
}


static int
declarator_is_simple_name(node_t *np)
{
    assert(node_is(np, "declarator"));
    return (np->nchild == 1 && direct_declarator_is_simple_name(np->child[0]));
}


static int
declarator_is_simple_pointer(node_t *np)
{
    assert(node_is(np, "declarator"));
    return
        (
            np->nchild == 2
        &&
            node_is(np->child[0], "pointer")
        &&
            (
                np->child[0]->nchild == 1
            ||
                (
                    np->child[0]->nchild == 2
                &&
                    node_is(np->child[0]->child[1], "type_qualifier_list")
                )
            )
        &&
            node_is(np->child[1], "direct_declarator")
        &&
            direct_declarator_is_simple_name(np->child[1])
        );
}


int
node_parameter_is_const_char_star(const node_t *np)
{
    assert(node_is(np, "parameter_declaration"));
    assert(np->nchild == 2);
    assert(node_is(np->child[0], "declaration_specifiers"));
    assert(node_is(np->child[1], "declarator"));

    return
        (
            grope_declspec(np->child[0]) == (IS_CONST | IS_CHAR)
        &&
            declarator_is_simple_pointer(np->child[1])
        );
}


int
node_parameter_is_pointer(const node_t *np)
{
    assert(node_is(np, "parameter_declaration"));
    assert(np->nchild == 2);
    assert(np->child[1]->nchild >= 1);
    return node_is(np->child[1]->child[0], "pointer");
}


static void
strsizecopy(char *dst, const char *src, size_t dst_size)
{
    const char      *ep;

    if (!dst_size)
        return;
    --dst_size;
    ep = memchr(src, '\0', dst_size);
    if (ep)
        dst_size = ep - src;
    memcpy(dst, src, dst_size);
    dst[dst_size] = '\0';
}


int
node_parameter_is_formatable(const node_t *parameter, char *fmt,
    size_t fmt_size)
{
    int             declspec;

    assert(node_is(parameter, "parameter_declaration"));
    if (node_parameter_is_const_char_star(parameter))
    {
        strsizecopy(fmt, "explain_buffer_pathname", fmt_size);
        return 1;
    }
    if (node_parameter_is_pointer(parameter))
    {
        strsizecopy(fmt, "explain_buffer_pointer", fmt_size);
        return 1;
    }

    assert(parameter->nchild == 2);
    if (!declarator_is_simple_name(parameter->child[1]))
        return 0;
    declspec = grope_declspec(parameter->child[0]);
    declspec &= ~(IS_CONST | IS_VOLATILE | IS_REGISTER | IS_AUTO);
    switch (declspec)
    {
    case IS_CHAR:
    case IS_CHAR | IS_SIGNED:
    case IS_CHAR | IS_UNSIGNED:
        strsizecopy(fmt, "explain_buffer_char", fmt_size);
        return 1;

    case IS_OFF_T:
        strsizecopy(fmt, "explain_buffer_off_t", fmt_size);
        return 1;

    case IS_SOCKLEN_T:
        strsizecopy(fmt, "explain_buffer_socklen_t", fmt_size);
        return 1;

    case IS_SHORT:
    case IS_SHORT | IS_SIGNED:
    case IS_SHORT | IS_INT:
    case IS_SHORT | IS_INT | IS_SIGNED:
        strsizecopy(fmt, "explain_buffer_short", fmt_size);
        return 1;

    case IS_SHORT | IS_UNSIGNED:
    case IS_SHORT | IS_INT | IS_UNSIGNED:
        strsizecopy(fmt, "explain_buffer_ushort", fmt_size);
        return 1;

    case IS_INT:
    case IS_INT | IS_SIGNED:
    case IS_SIGNED:
        strsizecopy(fmt, "explain_buffer_int", fmt_size);
        return 1;

    case IS_INT | IS_UNSIGNED:
    case IS_UNSIGNED:
        strsizecopy(fmt, "explain_buffer_uint", fmt_size);
        return 1;

    case IS_LONG:
    case IS_LONG | IS_SIGNED:
    case IS_LONG | IS_INT:
    case IS_LONG | IS_INT | IS_SIGNED:
        strsizecopy(fmt, "explain_buffer_long", fmt_size);
        return 1;

    case IS_LONG | IS_UNSIGNED:
    case IS_LONG | IS_INT | IS_UNSIGNED:
        strsizecopy(fmt, "explain_buffer_ulong", fmt_size);
        return 1;

    case IS_LONGLONG:
    case IS_LONGLONG | IS_SIGNED:
    case IS_LONGLONG | IS_INT:
    case IS_LONGLONG | IS_INT | IS_SIGNED:
        strsizecopy(fmt, "explain_buffer_long_long", fmt_size);
        return 1;

    case IS_LONGLONG | IS_UNSIGNED:
    case IS_LONGLONG | IS_INT | IS_UNSIGNED:
        strsizecopy(fmt, "explain_buffer_unsigned_long_long", fmt_size);
        return 1;

    case IS_DEV_T:
        strsizecopy(fmt, "explain_buffer_dev_t", fmt_size);
        return 1;

    case IS_PTRDIFF_T:
        strsizecopy(fmt, "explain_buffer_ptrdiff_t", fmt_size);
        return 1;

    case IS_SIZE_T:
        strsizecopy(fmt, "explain_buffer_size_t", fmt_size);
        return 1;

    case IS_SSIZE_T:
        strsizecopy(fmt, "explain_buffer_ssize_t", fmt_size);
        return 1;

    case IS_FLOAT:
    case IS_DOUBLE:
        strsizecopy(fmt, "%g", fmt_size);
        return 1;

    case IS_LONG | IS_DOUBLE:
        strsizecopy(fmt, "%Lg", fmt_size);
        return 1;

    case IS_TIME_T:
        strsizecopy(fmt, "explain_buffer_time_t", fmt_size);
        return 1;

    default:
        return 0;
    }
}


int
node_parameter_from_string(const node_t *parameter, char *fmt,
    size_t fmt_size)
{
    int             declspec;

    assert(node_is(parameter, "parameter_declaration"));
    if (node_parameter_is_const_char_star(parameter))
    {
        strsizecopy(fmt, "", fmt_size);
        return 1;
    }
    if (node_parameter_is_pointer(parameter))
    {
        strsizecopy(fmt, "explain_parse_pointer_or_die", fmt_size);
        return 1;
    }

    assert(parameter->nchild == 2);
    if (!declarator_is_simple_name(parameter->child[1]))
        return 0;
    declspec = grope_declspec(parameter->child[0]);
    declspec &= ~(IS_CONST | IS_VOLATILE | IS_REGISTER | IS_AUTO);
    switch (declspec)
    {
    case IS_CHAR:
    case IS_CHAR | IS_SIGNED:
        strsizecopy(fmt, "explain_parse_int", fmt_size);
        return 1;

    case IS_CHAR | IS_UNSIGNED:
        strsizecopy(fmt, "explain_parse_uint_or_die", fmt_size);
        return 1;

    case IS_OFF_T:
        strsizecopy(fmt, "explain_parse_off_t_or_die", fmt_size);
        return 1;

    case IS_SOCKLEN_T:
        strsizecopy(fmt, "explain_parse_socklen_t_or_die", fmt_size);
        return 1;

    case IS_SHORT:
    case IS_SHORT | IS_SIGNED:
    case IS_SHORT | IS_INT:
    case IS_SHORT | IS_INT | IS_SIGNED:
        strsizecopy(fmt, "explain_parse_int_or_die", fmt_size);
        return 1;

    case IS_SHORT | IS_UNSIGNED:
    case IS_SHORT | IS_INT | IS_UNSIGNED:
        strsizecopy(fmt, "explain_parse_uint_or_die", fmt_size);
        return 1;

    case IS_INT:
    case IS_INT | IS_SIGNED:
    case IS_SIGNED:
        strsizecopy(fmt, "explain_parse_int_or_die", fmt_size);
        return 1;

    case IS_INT | IS_UNSIGNED:
    case IS_UNSIGNED:
        strsizecopy(fmt, "explain_parse_uint_or_die", fmt_size);
        return 1;

    case IS_LONG:
    case IS_LONG | IS_SIGNED:
    case IS_LONG | IS_INT:
    case IS_LONG | IS_INT | IS_SIGNED:
        strsizecopy(fmt, "explain_parse_long_od_die", fmt_size);
        return 1;

    case IS_LONG | IS_UNSIGNED:
    case IS_LONG | IS_INT | IS_UNSIGNED:
        strsizecopy(fmt, "explain_parse_ulomg_or_die", fmt_size);
        return 1;

    case IS_LONGLONG:
    case IS_LONGLONG | IS_SIGNED:
    case IS_LONGLONG | IS_INT:
    case IS_LONGLONG | IS_INT | IS_SIGNED:
        strsizecopy(fmt, "explain_parse_longlong_or_die", fmt_size);
        return 1;

    case IS_LONGLONG | IS_UNSIGNED:
    case IS_LONGLONG | IS_INT | IS_UNSIGNED:
        strsizecopy(fmt, "explain_parse_ulonglong_or_die", fmt_size);
        return 1;

    case IS_PTRDIFF_T:
        strsizecopy(fmt, "explain_parse_ptrdiff_t_or_die", fmt_size);
        return 1;

    case IS_SIZE_T:
        strsizecopy(fmt, "explain_parse_size_t_or_die", fmt_size);
        return 1;

    case IS_SSIZE_T:
        strsizecopy(fmt, "explain_parse_ssize_t_or_die", fmt_size);
        return 1;

    case IS_FLOAT:
        strsizecopy(fmt, "explain_parse_float_or_die", fmt_size);
        return 1;

    case IS_DOUBLE:
        strsizecopy(fmt, "explain_parse_double_or_die", fmt_size);
        return 1;

    case IS_LONG | IS_DOUBLE:
        strsizecopy(fmt, "explain_parse_long_double_or_die", fmt_size);
        return 1;

    case IS_TIME_T:
        strsizecopy(fmt, "explain_parse_time_t_or_die", fmt_size);
        return 1;

    case IS_UID_T:
        strsizecopy(fmt, "explain_parse_uid_t_or_die", fmt_size);
        return 1;

    case IS_GID_T:
        strsizecopy(fmt, "explain_parse_gid_t_or_die", fmt_size);
        return 1;

    default:
        return 0;
    }
}


const char *
find_function_name(node_t *np)
{
    assert(node_is(np, "declaration"));
    np = np->child[1]->child[0]->child[0];

    assert(node_is(np, "declarator"));
    assert(np->nchild == 1 || np->nchild == 2);
    np = np->child[np->nchild - 1];
    assert(node_is(np, "direct_declarator"));

    if (np->nchild != 4 || !node_is_literal(np->child[1], "("))
    {
        not_a_func:
        explain_output_error_and_die("not a function declaration");
    }
    assert(node_is_literal(np->child[3], ")"));

    np = np->child[0];
    assert(node_is(np, "direct_declarator"));
    if (np->nchild != 1 || !np->child[0]->literal)
        goto not_a_func;

    return np->child[0]->literal;
}


/* vim: set ts=8 sw=4 et : */
