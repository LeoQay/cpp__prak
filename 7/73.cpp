#include <string>
#include <map>

typedef std::pair<std::string, std::string> rule_t;

std::multimap<std::string, std::string> g()
{
    std::multimap<std::string, std::string> grm;

    grm.insert(rule_t("S", "WWW"));
    grm.insert(rule_t("W", "c"));
    grm.insert(rule_t("W", "aW"));
    grm.insert(rule_t("W", "bW"));

    return grm;
}

