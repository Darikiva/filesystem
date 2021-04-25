#include "Controller.hpp"

#include <iostream>

#include "Utils.hpp"

namespace FS {

const std::unordered_map<std::string, std::pair<Controller::Command, int>>
    Controller::str_to_command = {
        {"cr", {Controller::Command::Create, 1}},
        {"de", {Controller::Command::Destroy, 1}},
        {"op", {Controller::Command::Open, 1}},
        {"cl", {Controller::Command::Close, 1}},
        {"rd", {Controller::Command::Read, 2}},
        {"wr", {Controller::Command::Write, 3}},
        {"sk", {Controller::Command::Seek, 2}},
        {"dr", {Controller::Command::Directory, 0}},
        {"sv", {Controller::Command::Save, 0}}};

auto Controller::convert(std::string str) -> std::pair<Controller::Command, int>
{
    if (str_to_command.find(str) == str_to_command.end())
    {
        return {Command::Invalid, -1};
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

void Controller::work(std::istream& in, std::ostream& out)
{
    std::string buffer;
    while (std::getline(in, buffer))
    {
        auto args = Utils::split(buffer);
        auto wow = parseCommand(args);
        if (wow == "exit")
            return;
        out << wow;
    }
}

std::string Controller::parseCommand(std::vector<std::string> args)
{
    std::string output = "Error";
    if (args.empty()) return output;
    auto cmd = convert(args[0]);
    if (cmd.second + 1 == args.size())
    {
        switch (cmd.first)
        {
            default:
            case Command::Invalid: {
                output = "invalid command";
            }
            break;
            case Command::Create: {
                std::string filename = args[1];
                Utils::lengthen(filename);
                auto res = filesystem->create(filename);
                if (res == FS::Status::Success)
                {
                    output = "file " + filename + " created";
                }
                else
                {
                    output = toString(res);
                }
            }
            break;
            case Command::Destroy: {
                std::string filename = args[1];
                Utils::lengthen(filename);
                auto res = filesystem->destroy(filename);
                if (res == FS::Status::Success)
                {
                    output = "file " + filename + " destroyed";
                }
                else
                {
                    output = toString(res);
                }
            }
            break;
            case Command::Open: {
                std::string filename = args[1];
                Utils::lengthen(filename);
                auto res = filesystem->open(filename);
                if (res.first == FS::Status::Success)
                {
                    output = "file " + filename + " opened, index = " + std::to_string(res.second);
                }
                else
                {
                    output = toString(res.first);
                }
            }
            break;
            case Command::Close: {
                int index = std::atoi(args[1].c_str());
                auto res = filesystem->close(index);
                if (res == FS::Status::Success)
                {
                    output = "file " + std::to_string(index) + " closed";
                }
                else
                {
                    output = toString(res);
                }
            }
            break;
            case Command::Read: {
                int index = std::atoi(args[1].c_str());
                int count = std::atoi(args[2].c_str());
                std::string mem_area(count, ' ');
                auto res = filesystem->read(index, mem_area.data(), count);
                output = std::to_string(res.second) + " bytes read: " + mem_area;
                if (res.first != FS::Status::Success)
                {
                    output += toString(res.first);
                }
            }
            break;
            case Command::Write: {
                int index = std::atoi(args[1].c_str());
                char ch = args[2][0];
                int count = std::atoi(args[3].c_str());
                std::string mem_area(count, ch);
                auto res = filesystem->write(index, mem_area.data(), count);
                if (res.first == FS::Status::Success)
                {
                    output = std::to_string(count) + " bytes written";
                }
                else
                {
                    output = toString(res.first);
                }
            }
            break;
            case Command::Seek: {
                int index = std::atoi(args[1].c_str());
                int pos = std::atoi(args[2].c_str());
                auto res = filesystem->lseek(index, pos);
                if (res.first == FS::Status::Success)
                {
                    output = "current position is " + std::to_string(res.second);
                }
                else
                {
                    output = toString(res.first);
                }
            }
            break;
            case Command::Directory: {
                output = "";
                for (const auto& [filename, length] : filesystem->directory())
                {
                    output += filename + " " + std::to_string(length) + ", ";
                }
            }
            break;
            case Command::Save: {
                std::cout << "save";
            }
            break;
        }
    }
    else
    {
        return "exit";
    }
    output += '\n';
    return output;
}

} // namespace FS