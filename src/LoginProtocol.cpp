#include "ProtocolDispatch.h"
// #include "mysql_driver.h"
// #include "mysql_connection.h"

bool DealProtocol(WorldPacket *pack){
    Log_Debug("玩家登录成功");

    uint16 length;
    *pack >> length;
    std::string account;
    std::string password;
    *pack >> account;
    *pack >> password;

    Log_Debug("%s : %s", account.c_str(), password.c_str());
    return true;
}
