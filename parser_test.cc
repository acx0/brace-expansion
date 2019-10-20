#include <gtest/gtest.h>

#include "parser.h"

namespace parser_test
{
    TEST(ParserTest, LiteralNodeWithNoAdjacency)
    {
        auto exp = "A";

        auto root = parser::build_parse_tree(exp);

        ASSERT_NE(nullptr, root);
        ASSERT_TRUE(root->is_literal_node());
        ASSERT_EQ("A", root->literal_value);
        ASSERT_EQ(nullptr, root->next);
    }

    TEST(ParserTest, LiteralNodeWithGroupAdjacency)
    {
        auto exp = "A{B}";

        auto root = parser::build_parse_tree(exp);

        ASSERT_NE(nullptr, root);
        ASSERT_TRUE(root->is_literal_node());
        ASSERT_EQ("A", root->literal_value);
        ASSERT_NE(nullptr, root->next);
        ASSERT_TRUE(root->next->is_group_node());
        ASSERT_EQ(1, root->next->subnodes->size());
        ASSERT_EQ("B", root->next->subnodes->at(0)->literal_value);
        ASSERT_EQ(nullptr, root->next->subnodes->at(0)->next);
        ASSERT_EQ(nullptr, root->next->next);
    }

    TEST(ParserTest, NodeGroupWithLiteralAdjacency)
    {
        auto exp = "{A}B";

        auto root = parser::build_parse_tree(exp);

        ASSERT_NE(nullptr, root);
        ASSERT_TRUE(root->is_group_node());
        ASSERT_EQ(1, root->subnodes->size());
        ASSERT_EQ("A", root->subnodes->at(0)->literal_value);
        ASSERT_EQ(nullptr, root->subnodes->at(0)->next);
        ASSERT_NE(nullptr, root->next);
        ASSERT_EQ("B", root->next->literal_value);
        ASSERT_EQ(nullptr, root->next->next);
    }

    TEST(ParserTest, NodeGroupWithGroupAdjacency)
    {
        auto exp = "{A}{B}";

        auto root = parser::build_parse_tree(exp);

        ASSERT_NE(nullptr, root);
        ASSERT_TRUE(root->is_group_node());
        ASSERT_EQ(1, root->subnodes->size());
        ASSERT_EQ("A", root->subnodes->at(0)->literal_value);
        ASSERT_EQ(nullptr, root->subnodes->at(0)->next);
        ASSERT_TRUE(root->next->is_group_node());
        ASSERT_EQ(1, root->next->subnodes->size());
        ASSERT_EQ("B", root->next->subnodes->at(0)->literal_value);
        ASSERT_EQ(nullptr, root->next->subnodes->at(0)->next);
    }

    TEST(ParserTest, MultiNodeGroupWithNoAdjacency)
    {
        auto exp = "{A,B}";

        auto root = parser::build_parse_tree(exp);

        ASSERT_NE(nullptr, root);
        ASSERT_TRUE(root->is_group_node());
        ASSERT_EQ(2, root->subnodes->size());
        ASSERT_EQ("A", root->subnodes->at(0)->literal_value);
        ASSERT_EQ(nullptr, root->subnodes->at(0)->next);
        ASSERT_EQ("B", root->subnodes->at(1)->literal_value);
        ASSERT_EQ(nullptr, root->subnodes->at(1)->next);
        ASSERT_EQ(nullptr, root->next);
    }

    TEST(ParserTest, SingleNodeGroupWithNoAdjacency)
    {
        auto exp = "{A}";

        auto root = parser::build_parse_tree(exp);

        ASSERT_NE(nullptr, root);
        ASSERT_TRUE(root->is_group_node());
        ASSERT_EQ(1, root->subnodes->size());
        ASSERT_EQ("A", root->subnodes->at(0)->literal_value);
        ASSERT_EQ(nullptr, root->subnodes->at(0)->next);
        ASSERT_EQ(nullptr, root->next);
    }

    TEST(ParserTest, ChainedGroupNodes)
    {
        auto exp = "{A}{C}{E}";

        auto root = parser::build_parse_tree(exp);

        ASSERT_NE(nullptr, root);
        ASSERT_TRUE(root->is_group_node());
        ASSERT_EQ(1, root->subnodes->size());
        ASSERT_EQ("A", root->subnodes->at(0)->literal_value);
        ASSERT_EQ(nullptr, root->subnodes->at(0)->next);
        ASSERT_TRUE(root->next->is_group_node());
        ASSERT_EQ(1, root->next->subnodes->size());
        ASSERT_EQ("C", root->next->subnodes->at(0)->literal_value);
        ASSERT_EQ(nullptr, root->next->subnodes->at(0)->next);
        ASSERT_TRUE(root->next->next->is_group_node());
        ASSERT_EQ(1, root->next->next->subnodes->size());
        ASSERT_EQ("E", root->next->next->subnodes->at(0)->literal_value);
        ASSERT_EQ(nullptr, root->next->next->subnodes->at(0)->next);
    }
}
