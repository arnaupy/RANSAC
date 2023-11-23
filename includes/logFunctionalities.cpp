#include <iostream>
#include "logFunctionalities.h"

MessagePrinter::MessagePrinter(std::string logsFilename, bool detachMode, bool logsMode)
{
    this->logsFilename = logsFilename;
    this->detachMode = detachMode;
    this->logsMode = logsMode;

    if (logsMode)
    {
        this->logsFile.open(logsFilename);
    }
}

void MessagePrinter::close()
{
    logsFile.close();
}
