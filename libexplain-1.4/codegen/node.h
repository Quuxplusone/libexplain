/*
 * libexplain - Explain errno values returned by libc functions
 * Copyright (C) 2008, 2009, 2011, 2012 Peter Miller
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

#ifndef CODEGEN_NODE_H
#define CODEGEN_NODE_H

#include <libexplain/ac/stddef.h>
#include <libexplain/ac/stdio.h>

typedef struct node_t node_t;
struct node_t
{
    long            reference_count;
    const char      *name;
    node_t          **child;
    size_t          nchild;
    size_t          nchild_max;
    const char      *literal;
};

node_t *node_new(const char *name);
node_t *node_copy(node_t *np);
void node_free(node_t *np);
node_t *node_new_literal(const char *text);
void node_push_back(node_t *parent, node_t *child);
void node_print(const node_t *np, FILE *fp);
void node_print_lisp(const node_t *np, FILE *fp);

int node_is(const node_t *np, const char *name);
int node_is_literal(const node_t *np, const char *text);
int node_is_a_literal(const node_t *np);

struct elastic_buffer_t; /* forward */

enum node_print_style_t
{
    node_print_style_normal,
    node_print_style_column16,
    node_print_style_function
};
typedef enum node_print_style_t node_print_style_t;

void node_print_sb(const node_t *np, struct elastic_buffer_t *sb,
    node_print_style_t style);

/**
  * The node_synth_or_die function is used to take the AST from the
  * command line, and synthesize a new AST that declares the matching
  * explain_*_or_die function.
  *
  * @param np
  *     The root of the AST to work on
  * @param use_return_value
  *     true if return value is to be used
  * @returns
  *     Pointer to new AST.
  */
node_t *node_synth_or_die(node_t *np, int use_return_value);

node_t *node_synth_on_error(node_t *np);
node_t *node_synth_result_variable(node_t *np);

/**
  * The function_returns_pointer function is used to determine whether
  * or not a declaration AST is of a function that returns a pointer.
  *
  * @param np
  *     The delcration node to inspect.
  * @returns
  *     bool; true if declaration is of a function that returns a point,
  *     false if not a function declaration, false if not a pointer
  *     declaration.
  */
int function_returns_pointer(const node_t *np);
int function_returns_int(const node_t *np);

int node_parameter_is_const_char_star(const node_t *np);
int node_parameter_is_formatable(const node_t *np, char *fmt, size_t fmt_size);
int node_parameter_from_string(const node_t *np, char *fmt, size_t fmt_size);
int node_parameter_is_pointer(const node_t *np);
const char *find_function_name(node_t *np);

#endif /* CODEGEN_NODE_H */
/* vim: set ts=8 sw=4 et : */
