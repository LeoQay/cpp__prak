#include <map>
#include <string>

typedef std::multimap<std::string, std::string> grm_t;
typedef std::pair<std::string, std::string> rule_t;

grm_t g1()
{
    grm_t grm;

    grm.insert(rule_t("S", "cA"));
    grm.insert(rule_t("A", "aA"));
    grm.insert(rule_t("A", "bA"));
    grm.insert(rule_t("A", "_"));

    return grm;
}

grm_t g2()
{
    grm_t grm;

    grm.insert(rule_t("S", "aS"));
    grm.insert(rule_t("S", "bS"));
    grm.insert(rule_t("S", "c"));

    return grm;
}