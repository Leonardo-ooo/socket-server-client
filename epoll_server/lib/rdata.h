#ifndef RDATA_H
#define RDATA_H

#include <iostream>

class Rdata
{
public:
    Rdata();

public:
    std::string action;
    std::string username;
    std::string content;
    std::string password;

public:
    std::string messagesend(std::string &data);
    bool parseMessage(std::string &data);
};

#endif // RDATA_H
