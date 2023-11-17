#include "command_buffer.hpp"
#include <iostream>


void CommandBuffer::addCommand(ICommand* cmd)
{
    mutex.lock();
    buffer.push(cmd);
    mutex.unlock();
}

ICommand* CommandBuffer::popCommand()
{
    mutex.lock();
    ICommand* cmd = nullptr;
    std::cout << "CommandBuffer::popCommand()\n";
    if (buffer.size() > 0)
    {
        cmd = buffer.front();
        buffer.pop();
    }
    mutex.unlock();
    return cmd;
}