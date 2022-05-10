#include <gtest/gtest.h>

#include "92_hfile.h"


class Test92 : public ::testing::Test
{

};


TEST_F(Test92, test_1)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'S', "aA"},
        {'S', "BDc"},
        {'A', "BAa"},
        {'A', "aB"},
        {'A', "b"},
        {'B', ""},
        {'D', "B"},
        {'D', "b"}
    };

    bool answer = checker.check(grm);

    ASSERT_FALSE(answer);
}


TEST_F(Test92, test_2)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'S', "bS"},
        {'S', "aT"},
        {'T', "bcT"},
        {'T', "cP"},
        {'T', ""},
        {'P', "cT"},
        {'P', "bB"},
        {'B', "cbB"},
        {'B', ""}
    };

    bool answer = checker.check(grm);

    ASSERT_TRUE(answer);
}

TEST_F(Test92, test_3)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'S', "BDc"},
        {'B', ""},
        {'D', "B"},
        {'D', "b"}
    };

    bool answer = checker.check(grm);

    ASSERT_TRUE(answer);
}

TEST_F(Test92, test_4)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'S', "bS"},
        {'S', "aAB"},
        {'A', "bcA"},
        {'A', "ccA"},
        {'A', ""},
        {'B', "cbB"},
        {'B', ""}
    };

    bool answer = checker.check(grm);

    ASSERT_FALSE(answer);
}

TEST_F(Test92, test_5)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'S', "kTp"},
        {'T', "aABC"},
        {'A', "bB"},
        {'B', "c"},
        {'B', ""},
        {'C', "dB"},
        {'C', "f"},
        {'C', ""}
    };

    bool answer = checker.check(grm);

    ASSERT_FALSE(answer);
}

TEST_F(Test92, test_6)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'S', "aA"},
        {'A', "BC"},
        {'A', "B"},
        {'C', "b"},
        {'C', ""},
        {'B', ""}
    };

    bool answer = checker.check(grm);

    ASSERT_FALSE(answer);
}

TEST_F(Test92, test_7)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'S', "cAd"},
        {'S', "d"},
        {'A', "aA"},
        {'A', ""}
    };

    bool answer = checker.check(grm);

    ASSERT_TRUE(answer);
}


TEST_F(Test92, test_8)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'S', "Bd"},
        {'B', "cAa"},
        {'B', "a"},
        {'A', "aA"},
        {'A', ""}
    };

    bool answer = checker.check(grm);

    ASSERT_FALSE(answer);
}

TEST_F(Test92, test_9)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'S', "BDC"},
        {'C', "Bd"},
        {'D', "aB"},
        {'D', "d"},
        {'B', "bB"},
        {'B', ""}
    };

    bool answer = checker.check(grm);

    ASSERT_FALSE(answer);
}

TEST_F(Test92, test_10)
{
    GrmValidityRDM checker;

    grm_t grm = {
        {'R', "MT"},
        {'T', "|R"},
        {'T', ""},
        {'M', "CM"},
        {'M', ""},
        {'C', "EP"},
        {'P', "*P"},
        {'P', "+P"},
        {'P', ""},
        {'E', "(R)"},
        {'E', "c"},
        {'E', "d"}
    };

    bool answer = checker.check(grm);

    ASSERT_TRUE(answer);
}

TEST_F(Test92, test_11)
{
    GrmValidityRDM checker;

    grm_t grm = {
    {'S', "Sa"},
    {'S', ""}
    };

    bool answer = checker.check(grm);

    ASSERT_FALSE(answer);
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}