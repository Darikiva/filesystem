#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "Disk.hpp"
#include "FileSystem.hpp"
#include "IOSystem.hpp"

namespace FS {

class Controller
{
public:
    enum class Command
    {
        Create,
        Destroy,
        Open,
        Close,
        Read,
        Write,
        Seek,
        Directory,
        Save,
        Invalid,
    };
    const static std::unordered_map<std::string, Command> str_to_command;

public:
    Controller(std::string save_name);
    void reset();
    void parse(std::string command);

private:
    Command convert(std::string str);

private:
    std::unique_ptr<Disk> disk;
    std::unique_ptr<IOSystem> iosystem;
    std::unique_ptr<FileSystem> filesystem;
};

} // namespace FS