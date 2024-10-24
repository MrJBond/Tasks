#include "Exceptions.h"

EndOfFile::EndOfFile()
{

}

EndOfFile::~EndOfFile()
{
}
const char* EndOfFile::what() const 
{
    return m_message.c_str();
}



ReadError::ReadError(std::string message) : m_message(message) {}

ReadError::~ReadError()
{
}

const char* ReadError::what() const
{
    return m_message.c_str();
}