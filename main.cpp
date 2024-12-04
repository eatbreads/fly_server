#include <utility/httplib.h>          // 新的 Web 库
#include <iostream>
#include <utility/json.hpp> // 新的 JSON 库
#include <common/Mapper.hpp>  // ORM 库
#include <entity.hpp>           // 包含 User 类的定义
#include <utility/jwt.h>      // JWT 工具
#include <api/userapi.h>
#include <api/flightapi.h>
#include <api/orderapi.h>
#include <api/sendapi.h>
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
    svr.Post("/api/users/login", handle_login);            // 登录接口
    svr.Post("/api/users/register", handle_register);      // 注册接口
    svr.Post("/api/users/update", handle_update_user_info); // 更改用户信息接口
    svr.Post("/api/users/logout", handle_logout);           // 用户注销接口
    svr.Post("/api/users/get", handle_get_user_by_id);      // 根据 email 获取用户信息接口

    // 航班管理接口
    svr.Post("/api/flights/search", handle_flight_search); // 查询航班信息
    svr.Post("/api/flights/create", handle_flight_create); // 创建航班信息
    svr.Post("/api/flights/update", handle_flight_update); // 修改航班信息
    svr.Post("/api/flights/delete", handle_flight_delete); // 删除航班信息

    // 订单管理接口
    svr.Post("/api/orders/create", handle_order_create);   // 创建订单
    svr.Post("/api/orders/search", handle_order_search);   // 查询订单
    svr.Post("/api/orders/update", handle_order_update);   // 更新订单状态

    svr.Post("/api/send", handle_send_email);

    // 服务器启动日志
    cout << "Server is running on 0.0.0.0" << endl;

    // 监听本地端口
    svr.listen("0.0.0.0", 8080);
    cout << "Server started successfully!" << endl;  // 如果这行没有打印出来，说明服务器未启动成功
    return 0;
}


