#pragma once

#include <iostream>
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
        Quit,
        Invalid,
    };
    const static std::unordered_map<std::string, std::pair<Command, int>> str_to_command;

public:
    Controller(std::string save_name);
    void reset();
    std::string parseCommand(std::vector<std::string> args);
    void work(std::istream& in, std::ostream& out);

private:
    auto convert(std::string str) -> std::pair<Controller::Command, int>;

private:
    std::unique_ptr<Disk> disk;
    std::unique_ptr<IOSystem> iosystem;
    std::unique_ptr<FileSystem> filesystem;
};

} // namespace FS