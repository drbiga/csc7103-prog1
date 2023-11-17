#include "filesystem.hpp"

#include <iostream>
#include <string.h>
#include <sstream>
#include <fstream>

FileSystemClient::FileSystemClient(std::string server_ip, int port)
{
    this->server_ip = server_ip;
    this->port = port;
    this->conn = new ClientConnection();
}

void FileSystemClient::start()
{
    bool done = false;
    
    bool connected = this->conn->connectToServer(this->server_ip, this->port);
    if (!connected)
    {
        std::cout << "There was a problem connecting to the server. Shutting down...\n";
        return;
    }

    this->printWelcomeMessage();
    while (!done)
    {
        std::cout << ">>> ";
        std::string cmdText, cmdFirstToken;
        std::getline(std::cin, cmdText);    
        std::getline(std::stringstream(cmdText), cmdFirstToken, ' ');

        if (cmdFirstToken == std::string("exit"))
        {
            done = true;
        }
        
        if (cmdFirstToken == std::string("help"))
        {
            this->printAvailableCommands();
        }
        else
        {
            std::string response = this->sendCommand(cmdText);
            std::cout << response << std::endl;

            // Follow up. Up and Down are the only commands
            // that require a follow up request (uploading or
            // downloading a file).
            if (this->serverAskedForFile(response))
            {
                std::cout << "Initiating file transfer\n";
                // Getting the filename from the response
                std::string filename;
                std::stringstream ss(response);
                std::getline(ss, filename, ' '); // discarding the upload-file token
                std::getline(ss, filename, ' '); // really getting the filename
                this->uploadFile(filename);
                std::cout << "File transfer complete\n";
                continue;
            }
            if (this->serverAskedToPushFile(response))
            {
                std::cout << "Initiating file transfer\n";
                // Getting the filename from the response
                std::string filename;
                std::stringstream ss(response);
                std::getline(ss, filename, ' '); // discarding the upload-file token
                std::getline(ss, filename, ' '); // really getting the filename
                this->downloadFile("downloaded-" + filename);
                std::cout << "File transfer complete\n";
                continue;
            }
        }
    }
}

void FileSystemClient::printWelcomeMessage()
{
    std::cout << "CSC 7103 - Network File System\n";
    std::cout << "==============================\n";
    this->printAvailableCommands();
}

void FileSystemClient::printAvailableCommands()
{
    std::cout << "Commands:\n";
    std::cout << "\tls - lists the content of the current directory\n";
    std::cout << "\tcd - change directory\n";
    std::cout << "\tcwd - prints current working directory\n";
    std::cout << "\tmkdir - make a new directory\n";
    std::cout << "\trm - remove a file or directory\n";
    std::cout << "\tup - upload a file\n";
    std::cout << "\tdown - download a file\n";
    std::cout << "\texit - exits this utility\n\n";
}

std::string FileSystemClient::sendCommand(std::string cmdText)
{
    return this->conn->sendRequest(cmdText);
}

bool FileSystemClient::serverAskedForFile(std::string response)
{
    std::string token;
    std::getline(std::stringstream(response), token, ' ');
    return token == std::string("upload-file");
}

void FileSystemClient::uploadFile(std::string filename)
{
    std::ifstream file(filename, std::ios::in);
    this->conn->sendPayload(&file);
}

bool FileSystemClient::serverAskedToPushFile(std::string response)
{
    std::string token;
    std::getline(std::stringstream(response), token, ' ');
    return token == std::string("download-file");
}

void FileSystemClient::downloadFile(std::string filename)
{
    this->conn->receivePayload(filename);
}
