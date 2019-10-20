#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <stdexcept>
#include <string>

#include "node.h"

/*
 * parser grammar:
 *     <expression>
 *         : <literal> <bracedExpressionTail>?
 *         | <bracedExpressionTail>
 *         ;
 *
 *     <bracedExpressionTail>
 *         : <bracedExpression> <expression>*
 *         ;
 *
 *     <bracedExpression>
 *         : '{' <expressionList> '}'
 *         ;
 *
 *     <expressionList>
 *         : <expression> ( ',' <expression> )*
 *         ;
 *
 *     <literal>
 *         : [a-zA-Z]+
 *         ;
 */

class parser
{
public:
    static std::unique_ptr<node> build_parse_tree(const std::string &exp);

private:
    static const char OPEN_BRACE;
    static const char CLOSE_BRACE;
    static const char COMMA;

    static bool is_valid_literal_char(char c);

    parser(const std::string &exp)
        : expression(exp), iter(expression.begin()), end(expression.end()), nesting_level(0)
    {
    }

    bool expression_consumed() const;
    bool accepted(char c) const;

    bool try_consume(char c);
    bool try_parse_expression(node *ctx_node);
    bool try_parse_braced_expression_tail(node *ctx_node);
    bool try_parse_braced_expression(node *ctx_node);
    bool try_parse_expression_list(node *ctx_node);
    bool try_parse_literal(std::string &literal);

    std::string expression;
    std::string::iterator iter;
    std::string::const_iterator end;
    int nesting_level;
};

#endif
