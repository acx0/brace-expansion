#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <sstream>
#include <string>
#include <vector>

#include "node.h"
#include "parser.h"

class expression
{
public:
    static std::string expand(const std::string &exp);
    static std::string expand(const node *const root);

private:
    static std::vector<std::string> cartesian_product(
        const std::vector<std::string> &first, const std::vector<std::string> &second);
    static std::vector<std::string> cartesian_product_helper(const node *const root);

    expression() = delete;
};

#endif
