#include "Exceptions.h"

EndOfFile::EndOfFile()
{

}

EndOfFile::~EndOfFile()
{
}
const char* EndOfFile::what() const 
{
    return message.c_str();
}



ReadError::ReadError(std::string message) : message(message) {}

ReadError::~ReadError()
{
}

const char* ReadError::what() const
{
    return message.c_str();
}