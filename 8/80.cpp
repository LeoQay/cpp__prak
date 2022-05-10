#include <map>
#include <string>

typedef std::multimap<std::string, std::string> grm_t;
typedef std::pair<std::string, std::string> rule_t;

grm_t g3()
{
    grm_t grm;

    grm.insert(rule_t("S", "d"));
    grm.insert(rule_t("S", "Cc"));
    grm.insert(rule_t("C", "Bb"));
    grm.insert(rule_t("B", "Sa"));

    return grm;
}

grm_t g4()
{
    grm_t grm;

    grm.insert(rule_t("S", "T_"));
    grm.insert(rule_t("S", "B_"));
    grm.insert(rule_t("T", "Tb"));
    grm.insert(rule_t("T", "Bb"));
    grm.insert(rule_t("B", "Ba"));
    grm.insert(rule_t("B", "a"));

    return grm;
}