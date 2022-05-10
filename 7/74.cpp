#include <string>
#include <map>

typedef std::pair<std::string, std::string> rule_t;

std::multimap<std::string, std::string> g()
{
    std::multimap<std::string, std::string> grm;

    grm.insert(rule_t("S", "aAB"));
    grm.insert(rule_t("S", "E"));
    grm.insert(rule_t("A", "dDA"));
    grm.insert(rule_t("A", ""));
    grm.insert(rule_t("B", "bE"));
    grm.insert(rule_t("B", "f"));
    grm.insert(rule_t("D", "eA"));
    grm.insert(rule_t("E", "fA"));
    grm.insert(rule_t("E", "g"));

    return grm;
}

