#ifndef FILESYSTEM_CLIENT
#define FILESYSTEM_CLIENT

#include <string>
#include "connection.hpp"

class FileSystemClient
{
    private:
        std::string server_ip;
        int port;
        ClientConnection* conn;
    public:
        FileSystemClient(std::string server_ip, int port);
        void start();
    private:
        void printWelcomeMessage();
        void printAvailableCommands();
        std::string sendCommand(std::string cmdText);
        void uploadFile(std::string filename);
        void downloadFile(std::string filename);
        bool serverAskedForFile(std::string response);
        bool serverAskedToPushFile(std::string response);
};

#endif
