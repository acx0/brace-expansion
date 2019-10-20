#include "parser.h"

const char parser::OPEN_BRACE = '{';
const char parser::CLOSE_BRACE = '}';
const char parser::COMMA = ',';

std::unique_ptr<node> parser::build_parse_tree(const std::string &exp)
{
    parser p(exp);
    auto root = node::create_uninitialized_node();
    bool parsed = false;
    try
    {
        parsed = p.try_parse_expression(root.get());
    }
    catch (const std::domain_error &e)
    {
    }

    return parsed ? std::move(root) : nullptr;
}

bool parser::is_valid_literal_char(char c)
{
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z');
}

bool parser::expression_consumed() const
{
    return iter == end;
}

bool parser::accepted(char c) const
{
    return !expression_consumed() && *iter == c;
}

bool parser::try_consume(char c)
{
    if (accepted(c))
    {
        ++iter;
        return true;
    }

    return false;
}

bool parser::try_parse_expression(node *ctx_node)
{
    if (!ctx_node->is_uninitialized_node())
    {
        throw std::domain_error("try_parse_expression: expected uninitialized node");
    }

    if (expression_consumed())
    {
        return false;
    }

    std::string literal;
    if (try_parse_literal(literal))
    {
        ctx_node->literal_value = literal;

        auto next_node = node::create_empty_group_node();
        if (try_parse_braced_expression_tail(next_node.get()))
        {
            ctx_node->next = std::move(next_node);
            return true;
        }

        return true;
    }

    auto next_node = node::create_empty_group_node();
    if (try_parse_braced_expression_tail(next_node.get()))
    {
        ctx_node->subnodes = std::move(next_node->subnodes);
        ctx_node->next = std::move(next_node->next);
        return true;
    }

    return false;
}

bool parser::try_parse_braced_expression_tail(node *ctx_node)
{
    if (!ctx_node->is_group_node())
    {
        throw std::domain_error("try_parse_braced_expression_tail: expected group node");
    }

    if (expression_consumed())
    {
        return false;
    }

    if (try_parse_braced_expression(ctx_node))
    {
        auto next_node = node::create_uninitialized_node();
        if (try_parse_expression(next_node.get()))
        {
            ctx_node->next = std::move(next_node);
        }

        return true;
    }

    return false;
}

bool parser::try_parse_braced_expression(node *ctx_node)
{
    if (!ctx_node->is_group_node())
    {
        throw std::domain_error("try_parse_braced_expression: expected group node");
    }

    if (!try_consume(OPEN_BRACE))
    {
        return false;
    }

    ++nesting_level;
    try_parse_expression_list(ctx_node);
    if (!try_consume(CLOSE_BRACE))
    {
        throw std::domain_error("try_parse_braced_expression: expected CLOSE_BRACE");
    }

    --nesting_level;
    return true;
}

bool parser::try_parse_expression_list(node *ctx_node)
{
    if (!ctx_node->is_group_node())
    {
        throw std::domain_error("try_parse_expression_list: expected group node");
    }

    // <expressionList> must contain at least one <expression>
    auto first_node = node::create_uninitialized_node();
    if (!try_parse_expression(first_node.get()))
    {
        throw std::domain_error("try_parse_expression_list: expected <expression>");
    }

    ctx_node->subnodes->push_back(std::move(first_node));
    if (accepted(CLOSE_BRACE))
    {
        return true;
    }
    else if (accepted(COMMA))
    {
        while (try_consume(COMMA))
        {
            // COMMA must always be followed by an <expression> as we don't allow empty values
            // inside a <bracedExpression>
            auto nth_node = node::create_uninitialized_node();
            if (!try_parse_expression(nth_node.get()))
            {
                throw std::domain_error(
                    "try_parse_expression_list: expected <expression> after COMMA");
            }

            ctx_node->subnodes->push_back(std::move(nth_node));
        }

        return true;
    }

    throw std::domain_error("try_parse_expression_list: expected COMMA or CLOSE_BRACE");
}

bool parser::try_parse_literal(std::string &literal)
{
    while (!expression_consumed())
    {
        char c = *iter;
        if (is_valid_literal_char(c))
        {
            literal += c;
            ++iter;
            continue;
        }

        if (nesting_level == 0)
        {
            // if we haven't encountered a <bracedExpression> yet, end of literal is determined by
            // start of new <bracedExpression>
            if (c == OPEN_BRACE)
            {
                break;
            }

            throw std::domain_error("try_parse_literal: expected OPEN_BRACE");
        }
        else
        {
            // once we've encountered at least one <bracedExpression>, end of literal is determined
            // by either start of new <bracedExpression>, end of current <bracedExpression>, or
            // COMMA
            if (c == OPEN_BRACE || c == CLOSE_BRACE || c == COMMA)
            {
                break;
            }

            throw std::domain_error(
                "try_parse_literal: expected OPEN_BRACE or CLOSE_BRACE or COMMA");
        }
    }

    return !literal.empty();
}
