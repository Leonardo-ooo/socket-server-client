#include "DBC.h"


DBC::DBC() 
{   
    conn = new MYSQL;
    res = NULL;
    field = NULL;
    row = NULL;
    mysql_init(conn);
    row_num = field_num = 0;
    if(!conn) { std::cerr << "init error! \n"; }
}

DBC::DBC(const char *host, const char *user, const char *pwd, const char* dbname, unsigned int port)
{
    res = NULL;
    field = NULL;
    row = NULL;
    mysql_init(conn);
    if(!conn) { std::cerr << "init error! \n"; }
    conn = mysql_real_connect(conn, host, user, pwd, dbname, port, NULL, 0);
    if(!conn) { std::cerr << "connect error! \n";}

}

DBC::~DBC()
{
    if(conn) mysql_close(conn);
}

bool DBC::Connect(const char *host, const char *user, const char *pwd, const char* dbname, unsigned int port)
{
    conn = mysql_real_connect(conn, host, user, pwd, dbname, port, NULL, 0);
    if(!conn) { std::cerr << "connect error! \n"; return false;}
    return true;
}

bool DBC::query(const char* sqlstr)
{
    if(mysql_query(conn, sqlstr)) return false;
    res = mysql_store_result(conn);
    if(!res) return false;
    
    row_num = mysql_num_rows(res);
    field_num = mysql_num_fields(res);

}

char* DBC::getfieldname(int index)
{
    field = mysql_fetch_field_direct(res, index);
    return field ->name;
}

bool DBC::nextline()
{
    row = mysql_fetch_row(res);
    if(!row) return false;
    return true;
}

