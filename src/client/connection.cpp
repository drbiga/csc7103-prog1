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
        // Sending file size to the server
        std::streampos begin = payload->tellg();
        payload->seekg(0, std::ios::end);
        std::streampos end = payload->tellg();
        long size = end - begin;
        long transformed_size = htonl(size);
        std::cout << "Sending file with size " << size << std::endl;
        send(this->socket_fd, &transformed_size, sizeof(transformed_size), 0);

        char chunk[CHUNK_SIZE];
        memset(chunk, 0, CHUNK_SIZE);
        // Resetting file to the beggining.
        payload->seekg(0, std::ios::beg);
        std::cout << "File is open\n";
        while (size > 0)
        {
            payload->read(chunk, CHUNK_SIZE);

            // std::cout << "Sending chunk: " << chunk << std::endl;
            std::cout << size - payload->gcount() << " bytes remaining\n";
            send(this->socket_fd, chunk, CHUNK_SIZE, 0);
            size -= CHUNK_SIZE;
        }
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
    std::ofstream file(filename, std::ios::binary);
    std::cout << "\tFile opened\n";

    // Receiving file size from client.
    long payload_size;
    read(this->socket_fd, &payload_size, sizeof(payload_size));
    payload_size = ntohl(payload_size);

    char buffer[CHUNK_SIZE];
    memset(buffer, 0, CHUNK_SIZE);
    std::cout << "Receiving " << payload_size << " bytes\n";
    while(payload_size > 0)
    {
        ssize_t size = read(this->socket_fd, buffer, CHUNK_SIZE);
        if (size == 0)
            break;
        std::cout << "Writing " << size << " bytes\n";
        std::cout << "ssize_t: " << size << " // int: " << (int) size << std::endl;
        payload_size -= (long) size;
        file.write(buffer, size);
    }
    std::cout << "Done writing file\n";

    file.close();
}

void ClientConnection::closeConnection()
{
    close(this->socket_fd);
}