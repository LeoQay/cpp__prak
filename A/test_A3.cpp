#include <gtest/gtest.h>

#include "A3.cpp"


class TestA3 : public ::testing::Test
{
public:
    static void check_pair(std::string inp, const std::string & correct)
    {
        Evaluator parser;
        bool ret = parser.parse(inp);
        ASSERT_EQ(ret, true);
        ASSERT_EQ(inp, correct);
    }

    static void check_bad(std::string inp)
    {
        Evaluator parser;
        bool ret = parser.parse(inp);
        ASSERT_EQ(ret, false);
    }
};


TEST_F(TestA3, test_1)
{
    check_pair("2",
               "2");
}

TEST_F(TestA3, test_2)
{
    check_pair("a=3,a",
               "3");
}

TEST_F(TestA3, test_3)
{
    check_bad("a");
}

TEST_F(TestA3, test_4)
{
    check_bad("a,b,c,1");
}

TEST_F(TestA3, test_5)
{
    check_pair("a=b=c=((2,4,5,b=1)),a-b-c",
               "-1");
}

TEST_F(TestA3, test_6)
{
    check_bad("a+b=2");
}

TEST_F(TestA3, test_7)
{
    check_bad("(a)=1");
}

TEST_F(TestA3, test_8)
{
    check_bad("a=b=c");
}

TEST_F(TestA3, test_9)
{
    check_bad("a=b=c,c=2");
}

TEST_F(TestA3, test_10)
{
    check_pair("9-3-4-2",
               "0");
}

TEST_F(TestA3, test_11)
{
    check_pair("1-((6-2)-1)",
               "-2");
}

TEST_F(TestA3, test_12)
{
    check_pair("((((a=3),((c=7)))),d=a=c=((9-(a-c))))",
               "13");
}

TEST_F(TestA3, test_13)
{
    check_pair("a=0,(a-2)-((a-2)-((a-2)-(a-(a-3))))",
               "-5");
}

TEST_F(TestA3, test_14)
{
    check_pair("a=0,((a-2)-((a-2)-(a-(a-3))))",
               "3");
}

TEST_F(TestA3, test_15)
{
    check_pair("1-(2-3,3)",
               "-2");
}

TEST_F(TestA3, test_16)
{
    check_pair("a=b=c=5,b=c=2-3,c=3-4-5",
               "-6");
}

TEST_F(TestA3, test_17)
{
    check_pair("a=2-3-4-5-6,a-0",
               "-16");
}

TEST_F(TestA3, test_18)
{
    check_pair("a=0,(a-2)-((a-2)-((a-2)-(a-(a-3))))",
               "-5");
}

TEST_F(TestA3, test_19)
{
    check_pair("a=0,((a-2)-((a-2)-(a-(a-3))))",
               "3");
}


int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

