# CSC 7103 - Programming Assignment

In order for the client program to be able to connect to the server from our computers, I changed the server IP address to the public IPV4 address of the classes server: 96.125.115.212

To execute the server, run `./run_server.sh`.

To execute the client, run `./run_client.sh`.

## Structure

The code is organized into two folders: server and client. There are five abstractions that compose this project. They are:

1. FileSystemServer
2. ServerConnection
3. ICommand
4. FileSystemClient
5. ClientConnection

The file system classes are responsible for handling the requests accordingly.

The connection classes handle socket programming.

The ICommand interface represents a file system command. This abstraction enable us to have a standardized interface to all file system commands, which means it is easy to extend and implement new commands. Every command is also encapsulated into its own class, so the code is well modularized.
