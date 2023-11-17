#include <iostream>

// Network
#include <sys/socket.h>
#include <netinet/in.h>

#include <unistd.h>
#include <string.h>

// Exceptions
#include <stdexcept>

#include <bits/stdc++.h>

// Files
#include <fstream>

#include "connection.hpp"


ServerConnection::ServerConnection(int port)
{
    this->port = port;
    // Create socket, bind and save in socket attribute
    this->prepareSocket();
}

ICommand* ServerConnection::receiveCommand()
{
    // Socket programming
    char buffer[1024];

    read(this->open_socket, buffer,
                   1024 - 1); // subtract 1 for the null
                              // terminator at the end

    ICommand* cmd;
    std::string cmdText(buffer);
    std::stringstream ss(cmdText);
    std::string cmdFirstToken;
    std::string cmdArgument;
    getline(ss, cmdFirstToken, ' ');

    if (cmdFirstToken == "exit")
    {
        cmd = new CommandExit();
    }
    else if (cmdFirstToken == "ls")
    {
        cmd = new CommandLs();
    }
    else if (cmdFirstToken == "cd")
    {
        getline(ss, cmdArgument, ' ');
        cmd = new CommandCd(cmdArgument);
    }
    else if (cmdFirstToken == "cwd")
    {
        cmd = new CommandCwd();
    }
    else if (cmdFirstToken == "mkdir")
    {
        getline(ss, cmdArgument, ' ');
        cmd = new CommandMkdir(cmdArgument);
    }
    else if (cmdFirstToken == "rm")
    {
        getline(ss, cmdArgument, ' ');
        cmd = new CommandRm(cmdArgument);
    }
    else if (cmdFirstToken == "up")
    {
        getline(ss, cmdArgument, ' ');
        cmd = new CommandUp(cmdArgument);
    }
    else if (cmdFirstToken == "down")
    {
        getline(ss, cmdArgument, ' ');
        cmd = new CommandDown(cmdArgument);
    }
    else
    {
        std::cout << cmdFirstToken << std::endl;
        throw std::invalid_argument("The received command is invalid. Type \"help\" if you need to review the available commands.");
    }

    return cmd;
}

void ServerConnection::sendResult(std::string result)
{
    std::cout << "\t" << result << std::endl;
    send(this->open_socket, result.c_str(), result.size()+1, 0); // +1 is needed for \0
}

void ServerConnection::receivePayload(std::string filename)
{
    std::cout << "\tReceive Payload:\n";
    char buffer[PAYLOAD_CHUNK_SIZE];
    std::ofstream file(filename);
    std::cout << "\tFile opened\n";
    bool done = false;
    while(!done)
    {
        memset(buffer, 0, PAYLOAD_CHUNK_SIZE);
        ssize_t size = read(this->open_socket, buffer, PAYLOAD_CHUNK_SIZE);
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

void ServerConnection::sendPayload(std::ifstream* payload)
{
    if (payload->is_open())
    {
        char chunk[PAYLOAD_CHUNK_SIZE];
        memset(chunk, 0, PAYLOAD_CHUNK_SIZE);
        std::cout << "File is open\n";
        while (payload->getline(chunk, PAYLOAD_CHUNK_SIZE))
        {
            std::cout << "Sending chunk: " << chunk;
            send(this->open_socket, chunk, PAYLOAD_CHUNK_SIZE, 0);
        }
        send(this->open_socket, EOF_PAYLOAD, sizeof EOF_PAYLOAD, 0);
        payload->close();
    }
    else
    {
        std::cout << "There was a problem while sending the file\n";
    }
}

void ServerConnection::prepareSocket()
{
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);
    char buffer[1024] = { 0 };
 
    // Creating socket file descriptor
    if ((this->server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
 
    // Forcefully attaching socket to the port
    if (setsockopt(this->server_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(this->port);
 
    // Forcefully attaching socket to the port
    if (bind(this->server_fd, (struct sockaddr*)&address,
             sizeof(address))
        < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(this->server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((this->open_socket
         = accept(this->server_fd, (struct sockaddr*)&address,
                  &addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
}

ServerConnection::~ServerConnection()
{
    this->closeSocket();
}

void ServerConnection::closeSocket()
{ 
    std::cout << "Closing socket\n";
    // closing the connected socket
    close(open_socket);
    // closing the listening socket
    close(server_fd);
}
