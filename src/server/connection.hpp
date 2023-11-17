#ifndef SERVER
#define SERVER

#include <string>
#include <fstream>

#include "command/command.hpp"

#define PAYLOAD_CHUNK_SIZE 256
#define EOF_PAYLOAD "EOF"

class ServerConnection
{
    private:
        int port;
        int open_socket, server_fd;
    public:
        ServerConnection(int port); // without command buffer
        ~ServerConnection();
        ICommand* receiveCommand();
        void sendResult(std::string text);
        void receivePayload(std::string filename); // upload file
        void sendPayload(std::ifstream* payload); // download file

    private:
        void prepareSocket();
        void closeSocket();
};

#endif