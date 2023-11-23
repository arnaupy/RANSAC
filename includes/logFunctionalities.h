#pragma ones
#include <iostream>
#include <fstream>

class MessagePrinter
{
public:
    std::string logsFilename;
    std::ofstream logsFile;
    bool detachMode;
    bool logsMode;
    MessagePrinter(std::string logsFilename = "logsFile.txt", bool detachMode = false, bool logsMode = false);
    template <typename T>
    void print(T message, bool endFile = false);
    void close();
};

template <typename T>
void MessagePrinter::print(T message, bool endFile)
{
    if (!detachMode)
    {
        std::cout << message;
        if (endFile)
        {
            std::cout << "\n";
        }
    }
    if (logsMode)
    {
        logsFile << message;
        if (endFile)
        {
            logsFile << "\n";
        }
    }
}
