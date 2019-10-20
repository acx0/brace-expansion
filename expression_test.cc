#include <gtest/gtest.h>

#include "expression.h"

namespace expression_test
{
    TEST(ExpressionTest, SuppliedValidCasesPass)
    {
        ASSERT_EQ("A B C", expression::expand("{A,B,C}"));
        ASSERT_EQ("AC AD BC BD", expression::expand("{A,B}{C,D}"));
        ASSERT_EQ("A BC BD", expression::expand("{A,B{C,D}}"));
        ASSERT_EQ("ABC", expression::expand("{ABC}"));
        ASSERT_EQ("ABC", expression::expand("ABC"));
    }

    TEST(ExpressionTest, SuppliedInvalidCasesFail)
    {
        ASSERT_EQ("", expression::expand("}ABC"));
        ASSERT_EQ("", expression::expand("{ABC"));
        ASSERT_EQ("", expression::expand("}{"));
        ASSERT_EQ("", expression::expand("{}"));
        ASSERT_EQ("", expression::expand("A,B,C"));
        ASSERT_EQ("", expression::expand("{A{B,C}"));
        ASSERT_EQ("", expression::expand("{A,}"));
    }

    TEST(ExpressionTest, LiteralsMustBeAsciiAlpha)
    {
        ASSERT_EQ("", expression::expand("7"));
        ASSERT_EQ("", expression::expand("{a,7}"));
        ASSERT_EQ("", expression::expand("{a,?}"));
        ASSERT_EQ("", expression::expand("{0,1}"));
        ASSERT_EQ("", expression::expand("*"));
        ASSERT_EQ("", expression::expand("a\n"));
        ASSERT_EQ("", expression::expand(" "));
        ASSERT_EQ("", expression::expand("{ }"));
    }

    TEST(ExpressionTest, ExpressionMustContainLiteral)
    {
        ASSERT_EQ("", expression::expand("{}"));
        ASSERT_EQ("", expression::expand("{{}}"));
        ASSERT_EQ("", expression::expand(""));
    }

    TEST(ExpressionTest, ListMustBeEnclosedInBraces)
    {
        ASSERT_EQ("", expression::expand("A,B"));
        ASSERT_EQ("", expression::expand("A,"));
    }

    TEST(ExpressionTest, BraceEnclosedListMustBeNonempty)
    {
        ASSERT_EQ("", expression::expand("{a,b}{}"));
        ASSERT_EQ("", expression::expand("{a,b}{{}}"));
    }

    TEST(ExpressionTest, BraceEnclosedListCannotContainEmptyItem)
    {
        ASSERT_EQ("", expression::expand("{a,b,}"));
        ASSERT_EQ("", expression::expand("{a,,c}"));
        ASSERT_EQ("", expression::expand("{,}"));
        ASSERT_EQ("", expression::expand("{,,,,}"));
    }

    TEST(ExpressionTest, RedundantNestingBracesAreIgnored)
    {
        ASSERT_EQ("A", expression::expand("{{{A}}}"));
        ASSERT_EQ("AB", expression::expand("{{{A}{B}}}"));
        ASSERT_EQ("Ax Ay Bx By", expression::expand("{{{{A,B}}}}{{x,y}}"));
        ASSERT_EQ("Ax Ay Bx By", expression::expand("{{{{{{{A,{{B}}}}}}{{x,y}}}}}"));
    }

    TEST(ExpressionTest, BracesMustBeBalanced)
    {
        ASSERT_EQ("", expression::expand("{A"));
        ASSERT_EQ("", expression::expand("A}"));
        ASSERT_EQ("", expression::expand("{{{A}}"));
        ASSERT_EQ("", expression::expand("{{A}{B}}}"));
        ASSERT_EQ("", expression::expand("{{{{A,B}}}{{x,y}}"));
        ASSERT_EQ("", expression::expand("{{{{{{{A,{{B}}}}}{{x,y}}}}}"));
    }

    TEST(ExpressionTest, Misc)
    {
        ASSERT_EQ("ABCD", expression::expand("A{B{C{D}}}"));
        ASSERT_EQ("ABCD", expression::expand("{A{B{C{D}}}}"));
        ASSERT_EQ("A BC BD", expression::expand("{A,B{C,D}}"));
        ASSERT_EQ("ACE ACF ACG ADE ADF ADG BCE BCF BCG BDE BDF BDG",
            expression::expand("{{A,B}{{C,{D}}}{E,{F},G}}"));
        ASSERT_EQ("ACE", expression::expand("{A}C{E}"));
        ASSERT_EQ("ACE", expression::expand("A{C}E"));
        ASSERT_EQ("ACE ACF ACG ADE ADF ADG BCE BCF BCG BDE BDF BDG",
            expression::expand("{A,B}{C,D}{E,F,G}"));
        ASSERT_EQ("aA aB zA zB",
            expression::expand("{{{{{{{{{{{{{{{{{{{{{{{{{a}}}}}}},{{{{{{{{z}}}}}}}}}{{{{{{A}}}}},{{"
                               "{{{B}}}}}}}}}}}}}}}}}}}}}}}"));
        ASSERT_EQ("a z A B",
            expression::expand("{{{{{{{{{{{{{{{{{{{{{{{{{a}}}}}}},{{{{{{{{z}}}}}}}}},{{{{{{A}}}}},{"
                               "{{{{B}}}}}}}}}}}}}}}}}}}}}}}"));
    }
}
