#include <utility/httplib.h>          // 新的 Web 库
#include <iostream>
#include <utility/json.hpp> // 新的 JSON 库
#include <common/Mapper.hpp>  // ORM 库
#include <entity.hpp>           // 包含 User 类的定义
#include <utility/jwt.h>      // JWT 工具
#include <api/userapi.h>
#include <api/flightapi.h>
using namespace httplib;
using namespace std;
using json = nlohmann::json;


// 函数声明
void handle_login(const Request& req, Response& res);
void handle_register(const Request& req, Response& res);
void handle_flight_search(const Request& req, Response& res); 
void handle_flight_create(const Request& req, Response& res);

int main() {
    Server svr;

    // 用户管理接口
    svr.Post("/api/users/login", handle_login);          // 登录接口
    svr.Post("/api/users/register", handle_register);    // 注册接口

    // 航班管理接口
    svr.Post("/api/flights/search", handle_flight_search);  // 查询航班信息
    svr.Post("/api/flights/create", handle_flight_create);  // 创建航班信息

    // 服务器启动日志
    cout << "Server is running on http://localhost:8080" << endl;

    // 监听本地端口
    svr.listen("localhost", 8080);

    return 0;
}
