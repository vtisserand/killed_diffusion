#include "logging.h"
#include <cstdarg>
#include <fstream>

void logging(const std::string &filename, const char *format, ...)
{
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    std::ofstream file;
    file.open(filename, std::ios_base::app);
    file << buffer << "\n";
    file.close();
}