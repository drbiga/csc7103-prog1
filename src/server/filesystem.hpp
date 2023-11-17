#ifndef FILESYSTEM_SERVER
#define FILESYSTEM_SERVER

#define CMD_BUFFER_SIZE 100

#include <vector>
#include "command/command.hpp"
#include "command/command_buffer.hpp"
#include "connection.hpp"

class FileSystemServer
{
    private:
        CommandBuffer* buffer;
        ServerConnection* conn;
    public:
        FileSystemServer();
        ~FileSystemServer();
        void listen(int port);
    private:
        void saveFile(std::string filename);
        void sendFile(std::string filename);
};

#endif
