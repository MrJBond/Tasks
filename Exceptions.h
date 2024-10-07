#pragma once
#include <exception>
#include <iostream>
#include <string>


class EndOfFile : public std::exception
{
private:
    const std::string  message = "The end of the file has been reached!\n\n";
public:
    EndOfFile();
    ~EndOfFile();

    const char* what() const override;
};

class ReadError : public std::exception 
{
private:
    std::string message;
public:
    ReadError(std::string message);
    ~ReadError();
    const char* what() const override;

};