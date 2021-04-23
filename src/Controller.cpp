#include "Controller.hpp"

#include <iostream>

namespace FS {

const std::unordered_map<std::string, Controller::Command> Controller::str_to_command = {
    {"cd", Controller::Command::Create},
    {"de", Controller::Command::Destroy},
    {"op", Controller::Command::Open},
    {"cl", Controller::Command::Close},
    {"rd", Controller::Command::Read},
    {"wr", Controller::Command::Write},
    {"sk", Controller::Command::Seek},
    {"dr", Controller::Command::Directory},
    {"sv", Controller::Command::Save}};

Controller::Command Controller::convert(std::string str)
{
    if (str_to_command.find(str) == str_to_command.end())
    {
        return Command::Invalid;
    }
    else
    {
        return str_to_command.at(str);
    }
}

Controller::Controller(std::string save_name)
{
    disk = std::make_unique<Disk>(save_name);
    iosystem = std::make_unique<IOSystem>(*disk);
    filesystem = std::make_unique<FileSystem>(*iosystem);
}

void Controller::reset()
{
    filesystem->reset();
}

void Controller::parse(std::string str_command)
{
    switch (convert(str_command))
    {
        case Command::Create:
            break;
        case Command::Destroy:
            break;
        case Command::Open:
            break;
        case Command::Close:
            break;
        case Command::Read:
            break;
        case Command::Write:
            break;
        case Command::Seek:
            break;
        case Command::Directory:
            break;
        case Command::Save:
            break;
        default:
            std::cout << "durak" << std::endl;
    }
}

} // namespace FS