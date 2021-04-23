#include <sstream>
#include <string>

// Search and remove whitespace from both ends of the string
static std::string TrimEnumString(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it)) { it++; }
    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit)) { rit++; }
    return std::string(it, rit.base());
}

static void SplitEnumArgs(const char* args, std::string array[], int number_in_enum)
{
    std::stringstream ss(args);
    std::string sub_str;
    int index = 0;
    while (ss.good() && (index < number_in_enum)) {
        getline(ss, sub_str, ',');
        array[index] = TrimEnumString(sub_str);
        index++;
    }
}

#define DECLARE_ENUM(enum_name, ...) \
    namespace FS { \
        enum enum_name { __VA_ARGS__, COUNT }; \
        static std::string _Strings[COUNT]; \
        static const char* ToString(enum_name e) { \
            if (_Strings[0].empty()) { SplitEnumArgs(#__VA_ARGS__, _Strings, COUNT); } \
            return _Strings[e].c_str(); \
        } \
        static enum_name FromString(const std::string& strEnum) { \
            if (_Strings[0].empty()) { SplitEnumArgs(#__VA_ARGS__, _Strings, COUNT); } \
            for (int i = 0; i < COUNT; i++) { if (_Strings[i] == strEnum) { return (enum_name)i; } } \
            return COUNT; \
        } \
    }
