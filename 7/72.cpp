#include <string>
#include <map>

typedef std::pair<std::string, std::string> rule_t;

std::multimap<std::string, std::string> g()
{
    std::multimap<std::string, std::string> grm;

    grm.insert(rule_t("S", "xSx"));
    grm.insert(rule_t("S", "ySy"));
    grm.insert(rule_t("S", "xx"));
    grm.insert(rule_t("S", "yy"));

    return grm;
}
