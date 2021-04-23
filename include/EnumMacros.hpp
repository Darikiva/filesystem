#include <sstream>
#include <string>

static std::string trimEnumString(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && isspace(*it))
    {
        it++;
    }
    std::string::const_reverse_iterator rit = s.rbegin();
    while (rit.base() != it && isspace(*rit))
    {
        rit++;
    }
    return std::string(it, rit.base());
}

static void splitEnumArgs(const char* args, std::string array[], int number_in_enum)
{
    std::stringstream ss(args);
    std::string sub_str;
    int index = 0;
    while (ss.good() && (index < number_in_enum))
    {
        getline(ss, sub_str, ',');
        array[index] = trimEnumString(sub_str);
        index++;
    }
}

#define DECLARE_ENUM(enum_name, ...)                        \
    namespace FS {                                          \
    enum enum_name                                          \
    {                                                       \
        __VA_ARGS__,                                        \
        COUNT                                               \
    };                                                      \
    static std::string _Strings[COUNT];                     \
    static const char* toString(enum_name e)                \
    {                                                       \
        if (_Strings[0].empty())                            \
        {                                                   \
            splitEnumArgs(#__VA_ARGS__, _Strings, COUNT);   \
        }                                                   \
        return _Strings[e].c_str();                         \
    }                                                       \
    static enum_name fromString(const std::string& strEnum) \
    {                                                       \
        if (_Strings[0].empty())                            \
        {                                                   \
            splitEnumArgs(#__VA_ARGS__, _Strings, COUNT);   \
        }                                                   \
        for (int i = 0; i < COUNT; i++)                     \
        {                                                   \
            if (_Strings[i] == strEnum)                     \
            {                                               \
                return (enum_name)i;                        \
            }                                               \
        }                                                   \
        return COUNT;                                       \
    }                                                       \
    }
