#include "expression.h"

std::string expression::expand(const std::string &exp)
{
    auto root = parser::build_parse_tree(exp);
    if (root == nullptr)
    {
        return "";
    }

    return expand(root.get());
}

std::string expression::expand(const node *const root)
{
    std::ostringstream oss;
    auto result = cartesian_product_helper(root);
    for (size_t i = 0; i < result.size(); ++i)
    {
        oss << result[i];
        if (i < result.size() - 1)
        {
            oss << " ";
        }
    }

    return oss.str();
}

std::vector<std::string> expression::cartesian_product(
    const std::vector<std::string> &first, const std::vector<std::string> &second)
{
    if (!first.empty() && second.empty())
    {
        return first;
    }

    if (first.empty() && !second.empty())
    {
        return second;
    }

    std::vector<std::string> result;
    for (auto &i : first)
    {
        for (auto &j : second)
        {
            result.push_back(i + j);
        }
    }

    return result;
}

std::vector<std::string> expression::cartesian_product_helper(const node *const root)
{
    if (root == nullptr)
    {
        return {};
    }

    if (root->is_literal_node())
    {
        return cartesian_product({root->literal_value}, cartesian_product_helper(root->next.get()));
    }

    std::vector<std::string> result;
    for (auto &subnode : *root->subnodes)
    {
        auto sub_result = cartesian_product_helper(subnode.get());
        result.insert(result.end(), sub_result.begin(), sub_result.end());
    }

    return cartesian_product(result, cartesian_product_helper(root->next.get()));
}
