#ifndef CLIENT_CONNECTION
#define CLIENT_CONNECTION

#include <string>

#define CHUNK_SIZE 256
#define EOF_PAYLOAD "EOF"

class ClientConnection
{
    private:
        int socket_fd;
    public:
        bool connectToServer(std::string server_ip, int port);
        std::string sendRequest(std::string commandText);
        void sendPayload(std::ifstream* payload); // upload file
        void receivePayload(std::string filename); // download file
        void closeConnection();
};

#endif
