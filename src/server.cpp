#include "server/filesystem.hpp"

#define SERVER_PORT 8000


int main()
{
    FileSystemServer* server = new FileSystemServer();
    server->listen(SERVER_PORT);
    return 0;
}
