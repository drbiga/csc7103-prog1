#include "command.hpp"

// Filesystem utilities: getcwd, chdir, ...
#include <filesystem>
#include <unistd.h>
#include <sys/stat.h>
#include <fstream> // files

// ============================================================================
std::string CommandLs::execute()
{
    std::string path = ".";
    std::string result = "";
    for (const auto & entry : std::filesystem::directory_iterator(path))
    {
        result.append(entry.path().string() + "\n");
    }

    return result;
}

// ============================================================================
CommandCd::CommandCd(std::string path)
{
    this->path = path;
}

std::string CommandCd::execute()
{
    int result = chdir(this->path.c_str());
    if (result == 0)
    {
        return std::string("");
    }
    else
    {
        return std::string("It was not possible to change to the specified directory");
    }
}

// ============================================================================
std::string CommandCwd::execute()
{
    char buffer[1024];
    getcwd(buffer, 1024);
    return std::string(buffer) + "\n";
}

// ============================================================================
CommandMkdir::CommandMkdir(std::string new_dir)
{
    this->new_dir = new_dir;
}

std::string CommandMkdir::execute()
{
    // Creating dir with permissions 751
    int status = mkdir(this->new_dir.c_str(), S_IRWXU | S_IRGRP | S_IXGRP | S_IXOTH);
    if (status == 0)
    {
        return std::string("Directory created successfully\n");
    }
    else
    {
        return std::string("Something went wrong while creating the directory\n");
    }
}

// ============================================================================
CommandRm::CommandRm(std::string path)
{
    this->path = path;
    // this->is_dir = is_dir;
}

std::string CommandRm::execute()
{
    int result = std::remove(path.c_str());
    if (result == 0)
    {
        return std::string("File removed successfully\n");
    }
    else
    {
        return std::string("There was a problem while removing your file\n");
    }
}

// ============================================================================
CommandUp::CommandUp(std::string filename)
{
    this->filename = filename;
}

std::string CommandUp::execute()
{
    return std::string("upload-file ") + this->filename;
}

// ============================================================================
CommandDown::CommandDown(std::string path)
{
    this->path = path;
}

std::string CommandDown::execute()
{
    return std::string("download-file ") + this->path;
}

// ============================================================================
// Dummy command to exit program
std::string CommandExit::execute()
{
    return std::string("Exit\n");
}
