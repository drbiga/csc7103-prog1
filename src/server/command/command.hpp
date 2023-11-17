#ifndef ICOMMAND
#define ICOMMAND

#include <string>
#include <sstream>

class ICommand
{
    public:
        virtual std::string execute() = 0;
};

class CommandLs : public ICommand
{
    public:
        std::string execute() override;
};

class CommandCd : public ICommand
{
    private:
        std::string path;
    public:
        CommandCd(std::string path);
        std::string execute() override;
};

class CommandCwd : public ICommand
{
    public:
        std::string execute() override;
};

class CommandMkdir : public ICommand
{
    private:
        std::string new_dir;
    public:
        CommandMkdir(std::string new_dir);
        std::string execute() override;
};

class CommandRm : public ICommand
{
    private:
        std::string path;
    public:
        CommandRm(std::string path);
        std::string execute() override;
};

class CommandUp : public ICommand
{
    private:
        std::string filename;
    public:
        CommandUp(std::string filename);
        std::string execute() override;
};

class CommandDown : public ICommand
{
    private:
        std::string path;
    public:
        CommandDown(std::string path);
        std::string execute() override;
};

class CommandExit : public ICommand
{
    public:
        std::string execute() override;
};

#endif