#include <iostream>
#include "filesystem.hpp"

#include <fstream>

FileSystemServer::FileSystemServer()
{
    // buffer = new CommandBuffer();
    // conn = new ServerConnection(buffer);
}

FileSystemServer::~FileSystemServer()
{
    delete conn;
    delete buffer;
}

void FileSystemServer::listen(int port)
{
    ICommand* cmd = nullptr;
    conn = new ServerConnection(port);
    bool done = false;
    while (!done)
    {
        std::cout << "Receiving command\n";
        try
        {
            cmd = conn->receiveCommand();
        }
        catch(const std::invalid_argument& e)
        {
            std::cout << "\t" << e.what() << std::endl;
            conn->sendResult(std::string(e.what()));
            continue;
        }
        catch(std::exception& e)
        {
            std::cout << "\tUnknown error thrown\n";
            conn->sendResult(std::string("Internal Server Error"));
            continue;
        }
        
        if (cmd != nullptr)
        {
            std::string result = cmd->execute();
            conn->sendResult(result);

            if (typeid(*cmd) == typeid(CommandUp))
            {
                std::cout << "\tInitiating file upload\n";
                // Reading filename
                std::string filename;
                std::stringstream ss(result);
                std::getline(ss, filename, ' '); // reads and discards upload-file token
                std::getline(ss, filename, ' '); // actually reads filename
                this->saveFile("server-" + filename);
                std::cout << "\tFile upload complete\n";
            }
            if (typeid(*cmd) == typeid(CommandDown))
            {
                std::cout << "\tInitiating file push to client\n";
                // Reading filename
                std::string filename;
                std::stringstream ss(result);
                std::getline(ss, filename, ' '); // reads and discards upload-file token
                std::getline(ss, filename, ' '); // actually reads filename
                this->sendFile(filename);
                std::cout << "\tFile push complete\n";
            }
            
            if (typeid(*cmd) == typeid(CommandExit))
            {
                delete conn;
                done = true;
            }
            
            delete cmd;
        }
        else
        {
            std::cout << "\tNull pointer returned from connection\n";
        }
    }
}

void FileSystemServer::saveFile(std::string filename)
{
    this->conn->receivePayload(filename);
}

void FileSystemServer::sendFile(std::string filename)
{
    std::ifstream file(filename, std::ios::in);
    this->conn->sendPayload(&file);
}
