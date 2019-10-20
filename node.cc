#include "node.h"

const std::string node::EMPTY_LITERAL_NODE_VALUE = "?";
const node::empty_group_tag node::EMPTY_GROUP;

std::unique_ptr<node> node::create_uninitialized_node()
{
    return std::unique_ptr<node>(new node());
}

std::unique_ptr<node> node::create_empty_literal_node()
{
    return std::unique_ptr<node>(new node(EMPTY_LITERAL_NODE_VALUE));
}

std::unique_ptr<node> node::create_literal_node(const std::string &literal)
{
    return std::unique_ptr<node>(new node(literal));
}

std::unique_ptr<node> node::create_empty_group_node()
{
    return std::unique_ptr<node>(new node(EMPTY_GROUP));
}

bool node::is_uninitialized_node() const
{
    return subnodes == nullptr && literal_value.empty();
}

bool node::is_literal_node() const
{
    return !literal_value.empty();
}

bool node::is_group_node() const
{
    return subnodes != nullptr;
}
