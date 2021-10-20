#include "rdata.h"
#include <stack>
#include <QDebug>

Rdata::Rdata()
{

}

std::string Rdata::messagesend(std::string &data)
{
    std::string res = "";
    res += "<data><action>message</action><content>";
    for (int i = 0; i < int(data.size()); ++ i){
        if (data[i]== '<' || data[i] == '>' || data[i] == '$')
            res+= '$';
        res += data[i];
    }
    res += "</content></data>";
    return res;
}

bool Rdata::parseMessage(std::string &data)
{
    std::stack<std::string> Label;
    std::string cur = "";
    for(int i = 0; i < int(data.size()); ++ i){
        if (data[i] == '$' && i + 1 < data.size()) {
            cur += data[++ i];
        }
        else if (data[i] == '>') {
            cur += data[i];
            if(cur[1] == '/') {
                Label.pop();
            }
            else {
                Label.push(cur);
            }
            cur = "";
        }
        else if (data[i] == '<' && i + 1 < int(data.size()) && data[i + 1] == '/') {
            if (!Label.empty()) {
                std::string LastLabel = Label.top();
                if (LastLabel == "<action>") {
                    action = cur;
                }
                else if (LastLabel == "<username>") {
                    username = cur;
                }
                else if (LastLabel == "<content>") {
                    content = cur;
                }
            }
            cur = "<";
        }
        else {
            cur += data[i];
        }
        if(Label.empty())
            qDebug() << QString::fromStdString(cur);
        else
            qDebug() << QString::fromStdString(cur) << " " << QString::fromStdString(Label.top()) << "\n";
    }
    if (Label.empty()) {
        return true;
    }
    else {
        return false;
    }
}
