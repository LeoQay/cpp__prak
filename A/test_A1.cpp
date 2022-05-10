#include <gtest/gtest.h>

#include "A1.cpp"


class TestA1 : public ::testing::Test
{
public:
    static void check_pair(std::string inp, const std::string & correct)
    {
        Evaluator parser;
        bool ret = parser.parse(inp);
        ASSERT_EQ(ret, true);
        ASSERT_EQ(inp, correct);
    }

    static void check_bad_syntax(std::string inp)
    {
        Evaluator parser;
        bool ret = parser.parse(inp);
        ASSERT_EQ(ret, false);
    }
};


TEST_F(TestA1, test_1)
{
    check_pair("a+b+c*b*s*(a+(b+d*(a+w)*(q+w)))",
               "a+b+c*b*s*(a+b+d*(a+w)*(q+w))");
}

TEST_F(TestA1, test_2)
{
    check_pair("a+(b*c)", "a+b*c");
}

TEST_F(TestA1, test_3)
{
    check_pair("(b*c)+a", "b*c+a");
}

TEST_F(TestA1, test_4)
{
    check_pair("(a+b)*(d+e)", "(a+b)*(d+e)");
}

TEST_F(TestA1, test_5)
{
    check_pair("(a+b)*v*(d+e)", "(a+b)*v*(d+e)");
}

TEST_F(TestA1, test_6)
{
    check_pair("w*((a))",
               "w*a");
}

TEST_F(TestA1, test_7)
{
    check_pair("(((a+b)*c)+b+c+(b+c))*d+a+b+a*b*(a*b+c)",
               "((a+b)*c+b+c+b+c)*d+a+b+a*b*(a*b+c)");
}

TEST_F(TestA1, test_8)
{
    check_pair("a+(b+c)", "a+b+c");
}

TEST_F(TestA1, test_9)
{
    check_pair("a+(b+c)", "a+b+c");
}

TEST_F(TestA1, test_10)
{
    check_pair("a", "a");
}

TEST_F(TestA1, test_11)
{
    check_pair("a*(b+c)*d", "a*(b+c)*d");
}

TEST_F(TestA1, test_12)
{
    check_pair("(a+b)+(q*s)*(x+o)+(((a*b+c)*d+w)*q+p)*c",
               "a+b+q*s*(x+o)+(((a*b+c)*d+w)*q+p)*c");
}

TEST_F(TestA1, test_13)
{
    check_pair("a*(b+c)+(c+a)*e+q*(d+w)*r",
               "a*(b+c)+(c+a)*e+q*(d+w)*r");
}

TEST_F(TestA1, test_14)
{
    check_pair("c*((s+q))",
               "c*(s+q)");
}

TEST_F(TestA1, test_15)
{
    check_pair("((a+f))*q", "(a+f)*q");
}

TEST_F(TestA1, test_16)
{
    check_pair("((a))*q", "a*q");
}

TEST_F(TestA1, test_17)
{
    check_pair("(((a+v)))*e", "(a+v)*e");
}

TEST_F(TestA1, test_18)
{
    check_pair("(((((((a))))*(((((a+v))))))))", "a*(a+v)");
}

TEST_F(TestA1, test_19)
{
    check_pair("c*(((((((a))))*(((((a+v))))))))", "c*a*(a+v)");
}

TEST_F(TestA1, test_20)
{
    check_pair("(((((((a))))*(((((a+v))))))))*c", "a*(a+v)*c");
}

TEST_F(TestA1, test_21)
{
    check_pair("(((((((a))))+(((((a+v))))))))", "a+a+v");
}

TEST_F(TestA1, test_22)
{
    check_pair("c+(((((((a))))*(((((a+v))))))))", "c+a*(a+v)");
}

TEST_F(TestA1, test_23)
{
    check_pair("(((((((a))))*(((((a+v))))))))+c", "a*(a+v)+c");
}

TEST_F(TestA1, test_24)
{
    check_pair("((((((((a)))*(((b+c)))*(((d)))))+((((((q+w)))*((((u+p)))))))+((((c))*((p)))))))",
               "a*(b+c)*d+(q+w)*(u+p)+c*p");
}

TEST_F(TestA1, test_25)
{
    check_bad_syntax("");
}

TEST_F(TestA1, test_26)
{
    check_bad_syntax("a+");
}

TEST_F(TestA1, test_27)
{
    check_bad_syntax("+a");
}

TEST_F(TestA1, test_28)
{
    check_bad_syntax("(a");
}

TEST_F(TestA1, test_29)
{
    check_bad_syntax("a)");
}

TEST_F(TestA1, test_30)
{
    check_bad_syntax("((a+())");
}

TEST_F(TestA1, test_31)
{
    std::string str = "_";

    for (int i = 0; i < 26; i++)
    {
        str[0] = (char) ('A' + i);
        check_pair(str, str);
    }
}

TEST_F(TestA1, test_32)
{
    check_bad_syntax("a + b");
}

TEST_F(TestA1, test_33)
{
    check_pair("(((((a+b)*(c+d)*(q+s)*(w*e)*(f+s)))))",
               "(a+b)*(c+d)*(q+s)*w*e*(f+s)");
}



int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}
