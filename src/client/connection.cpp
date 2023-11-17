// Network
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Interaction
#include <unistd.h>
#include <iostream>

// Strings
#include <string.h>

// Files
#include <fstream>

#include "connection.hpp"

bool ClientConnection::connectToServer(std::string server_ip, int port)
{
    int status;
    struct sockaddr_in serv_addr;
    char buffer[1024] = { 0 };
    this->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (this->socket_fd < 0) {
        printf("\n Socket creation error \n");
        return false;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    // std::cout << "Address:" << server_ip.c_str() << std::endl;
    printf("Address: %s", server_ip.c_str());
    if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr)
        <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return false;
    }
 
    if ((status
         = connect(this->socket_fd, (struct sockaddr*)&serv_addr,
                   sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return false;
    }

    return true;
}

std::string ClientConnection::sendRequest(std::string cmdText)
{
    int valread;
    char buffer[1024];
    send(this->socket_fd, cmdText.c_str(), cmdText.size()+1, 0);
    valread = read(this->socket_fd, buffer, 1024 - 1); // subtract 1 for the null terminator at the end
 
    // closing the connected socket
    return std::string(buffer);
}

void ClientConnection::sendPayload(std::ifstream* payload)
{
    if (payload->is_open())
    {
        char chunk[CHUNK_SIZE];
        memset(chunk, 0, CHUNK_SIZE);
        std::cout << "File is open\n";
        while (payload->getline(chunk, CHUNK_SIZE))
        {
            std::cout << "Sending chunk: " << chunk;
            send(this->socket_fd, chunk, CHUNK_SIZE, 0);
        }
        send(this->socket_fd, EOF_PAYLOAD, sizeof EOF_PAYLOAD, 0);
        payload->close();
    }
    else
    {
        std::cout << "There was a problem while sending the file\n";
    }
}

void ClientConnection::receivePayload(std::string filename)
{
    std::cout << "\tReceive Payload:\n";
    char buffer[CHUNK_SIZE];
    std::ofstream file(filename);
    std::cout << "\tFile opened\n";
    bool done = false;
    while(!done)
    {
        memset(buffer, 0, CHUNK_SIZE);
        ssize_t size = read(this->socket_fd, buffer, CHUNK_SIZE);
        if (std::string(buffer) == EOF_PAYLOAD)
            done = true;
        else
        {
            std::cout << "Writing " << buffer << std::endl;
            file << buffer << std::endl;
            std::cout << "Size: " << size << std::endl;
        }
    }
    std::cout << "Done writing file\n";

    file.close();
}

void ClientConnection::closeConnection()
{
    close(this->socket_fd);
}