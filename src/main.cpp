// #include "WorldPacket.h"
#include <iostream>
#include <stdio.h>
#include <jdbc/mysql_connection.h>
#include <jdbc/mysql_driver.h>
#include <jdbc/cppconn/driver.h>
#include <jdbc/cppconn/statement.h>

// g++ -g -o0 -I/usr/local/include -I/usr/local/boost/include -c main.cpp -o main.o
// g++ -g -o0 -L/usr/lib -L/usr/local/mysql/lib main.o -o test -lmysqlcppconn

// g++ -o test_mysql $(mysql_config --cflags) main.cpp $(mysql_config --libs) -lmysqlcppconn
int main(int argc, char const *argv[])
{
    sql::Driver *driver;
    sql::Connection *conn;
    sql::Statement *state;
    sql::ResultSet *result;

    bool asd = true;
    std::string tmp(asd? "true":"false");
    std::cout << tmp << std::endl;

    driver = get_driver_instance();
    std::cout << __LINE__ << std::endl;
    sql::SQLString url("tcp://192.168.1.2:3307");
    sql::SQLString user("root");
    sql::SQLString password("123457");
    conn = driver->connect(url, user, password);
    std::cout << __LINE__ << std::endl;
    state = conn->createStatement();
    std::cout << __LINE__ << std::endl;
    state->execute("use game");
    std::cout << __LINE__ << std::endl;
    result = state->executeQuery("select * from user");
    std::cout << __LINE__ << std::endl;
    for(; result->next();)
    {
        std::cout << result->getInt("user_id")<< std::endl;
        // std::cout << result->getString("content");
    }

    state->close();
    conn->close();

    // delete *result;
    // delete *state;
    // delete *conn;
    // WorldPacket w_packet;
    // WorldPacket *packet = &w_packet;
    // packet->clear();
    // // packet->reserve(64);
    // packet->SetOpcode(0x1234);
    // *packet << uint16(0x1234) << uint16(0);
    // *packet << uint8(32);
    // *packet << uint16(567);
    // *packet << (std::string)"this is a packet!";
    // packet->SetLength(packet->size());

    // // packet->SetOffset(0, --tmp);
    //         // << (std::string)"this is a packet";
    // // packet.SetLength(packet.size());
    // std::cout << "size: " << packet->size() << std::endl;
    // std::string tmp_str;
    // // uint8 message[30];
    // // *packet >> tmp_str;
    // std::cout << "contents: " << packet->Read<uint16>() << std::endl;

    // uint16 proto_id;
    // uint16 length;
    // uint8 a;
    // uint16 b;
    // std::string get_str;
    // *packet >> proto_id;
    // *packet >> length;

    // *packet >> a;
    // *packet >> b;
    // *packet >> get_str;

    // std::cout << "====================\n";
    // // std::cout << a << " " << b << " " << get_str << packet.size() << std::endl;
    // // printf("%d %d %s\n", a, b, get_str.c_str());
    // printf("%x %u %u %u\n%s\n", proto_id, length, a, b, get_str.c_str());


// class Stul;
// class Teacher;
// class Filter


// User getUserById(uint16 user_id)
// {
//     sql();
// }
// User getUserByName(uint16 user_id)
// {
//     sql();
// }

// // typedef<typename T>
// class Condition
// {
// private:
//     std::string column_name;
//     std::string regexp;
//     std::string value;
//     // int value;
// }



// student.age ->int
// int->string;
// private:

//     column  regexp  value
//     name    ==      'geshengquan'
//     age     >       18

//     Condition

//     result = query(conditions, tableName);
    

//     query(Condition []conditions, String tableName){
//         String sql = "select from " + tableName;
//         for condition in conditions:
//             sql += generalSql(condition)
//         return executeSql(sql);
//     }

//     generalSql(Condition condition){
//         return condition.column + condition.regexp + condition.value;
//     }
    return 0;
}


// 账号，密码。登录，注册
// 进入游戏
// 角色列表（包含区服）
// 新建角色，删除角色，开始游戏
    // 新建角色--选择职业--选择区服--输入角色名（判断）进入游戏，