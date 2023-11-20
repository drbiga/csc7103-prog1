#include "client/filesystem.hpp"

int main()
{
    FileSystemClient* client = new FileSystemClient(std::string("96.125.115.212"), 8000);
    client->start();
    return 0;
}
