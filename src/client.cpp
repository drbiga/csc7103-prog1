#include "client/filesystem.hpp"

int main()
{
    FileSystemClient* client = new FileSystemClient(std::string("127.0.0.1"), 8000);
    client->start();
    return 0;
}
