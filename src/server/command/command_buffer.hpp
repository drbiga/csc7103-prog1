#ifndef COMMAND_BUFFER
#define COMMAND_BUFFER

#include <queue>
#include <mutex>
#include "command.hpp"

class CommandBuffer
{
    private:
        std::mutex mutex;
        std::queue<ICommand*> buffer;
    public:
        void addCommand(ICommand* cmd);
        ICommand* popCommand();
};

#endif
