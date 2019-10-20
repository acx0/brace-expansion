#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <vector>

class node
{
public:
    static std::unique_ptr<node> create_uninitialized_node();
    static std::unique_ptr<node> create_empty_literal_node();
    static std::unique_ptr<node> create_literal_node(const std::string &literal);
    static std::unique_ptr<node> create_empty_group_node();

    bool is_uninitialized_node() const;
    bool is_group_node() const;
    bool is_literal_node() const;

    std::unique_ptr<std::vector<std::unique_ptr<node>>> subnodes;
    std::unique_ptr<node> next;
    std::string literal_value;

private:
    class empty_group_tag {};

    static const std::string EMPTY_LITERAL_NODE_VALUE;
    static const empty_group_tag EMPTY_GROUP;

    node() = default;
    node(const std::string &value)
        : literal_value(value) {}
    node(const empty_group_tag &)
        : subnodes(std::make_unique<std::vector<std::unique_ptr<node>>>()) {}
};

#endif
