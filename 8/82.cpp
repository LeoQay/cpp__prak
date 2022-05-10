#include <map>
#include <string>

typedef std::multimap<std::string, std::string> grm_t;
typedef std::pair<std::string, std::string> rule_t;

grm_t g3()
{
    grm_t grm;

    grm.insert(rule_t("S", "T_"));
    grm.insert(rule_t("T", "Ta"));
    grm.insert(rule_t("T", "Aa"));
    grm.insert(rule_t("T", "a"));
    grm.insert(rule_t("A", "Ab"));
    grm.insert(rule_t("A", "Tb"));
    grm.insert(rule_t("A", "b"));

    return grm;
}

grm_t g4()
{
    grm_t grm;

    grm.insert(rule_t("S", "aA"));
    grm.insert(rule_t("A", "aB"));
    grm.insert(rule_t("B", "aB"));
    grm.insert(rule_t("B", "bA"));
    grm.insert(rule_t("A", "_"));
    grm.insert(rule_t("B", "_"));

    return grm;
}
