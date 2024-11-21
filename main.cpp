#include <utility/httplib.h>          // 新的 Web 库
#include <iostream>
#include <utility/json.hpp> // 新的 JSON 库
#include <common/Mapper.hpp>  // ORM 库
#include <entity.hpp>           // 包含 User 类的定义
#include <utility/jwt.h>      // JWT 工具

using namespace httplib;
using namespace std;
using json = nlohmann::json;

// JWT 密钥
const string SECRET_KEY = "your-256-bit-secret";

// 登录接口
void handle_login(const Request& req, Response& res) {
    json responseJson;

    // 检查请求是否是 POST 方法
    if (req.method != "POST") {
        res.status = 405; // 方法不允许
        responseJson = {{"message", "Not a POST method"}, {"token", ""}};
        res.set_content(responseJson.dump(), "application/json");
        return;
    }

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("username") || !requestJson.contains("password")) {
            res.status = 400;
            responseJson = {{"message", "Missing username or password"}, {"token", ""}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        string username = requestJson["username"];
        string password = requestJson["password"];

        // 使用 ORM 查询数据库
        Mapper<User> userMapper;
        Example<User> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&User::username, username);

        // 获取用户
        auto users = userMapper.selectByExample(example);
        if (users.empty()) {
            res.status = 401;
            responseJson = {{"message", "User does not exist"}, {"token", ""}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        User user = users[0];
        if (user.password != password) {
            res.status = 401;
            responseJson = {{"message", "Incorrect password"}, {"token", ""}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 生成 JWT Token
        JWT jwt(SECRET_KEY);
        string header = R"({"alg":"HS256","typ":"JWT"})";
        string payload = R"({"sub":")" + to_string(user.id) + R"(","name":")" + user.username + R"("})";
        string token = jwt.generate(header, payload, 3600);

        res.status = 200;
        responseJson = {{"message", "Login successful"}, {"token", token}};
        res.set_content(responseJson.dump(), "application/json");
    } catch (const json::exception& e) {
        res.status = 400;
        responseJson = {{"message", "Invalid JSON format"}, {"token", ""}};
        res.set_content(responseJson.dump(), "application/json");
    }
}

// 注册接口
void handle_register(const Request& req, Response& res) {
    json responseJson;

    // 检查请求是否是 POST 方法
    if (req.method != "POST") {
        res.status = 405;
        responseJson = {{"message", "Not a POST method"}, {"userId", ""}};
        res.set_content(responseJson.dump(), "application/json");
        return;
    }

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("username") || !requestJson.contains("password") || 
            !requestJson.contains("email") || !requestJson.contains("phone") || 
            !requestJson.contains("age") || !requestJson.contains("gender") || 
            !requestJson.contains("idCard")) {
            res.status = 400;
            responseJson = {{"message", "Missing or invalid parameters"}, {"userId", ""}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        string username = requestJson["username"];
        string password = requestJson["password"];
        string email = requestJson["email"];
        string phone = requestJson["phone"];
        int age = requestJson["age"];
        string gender = requestJson["gender"];
        string idCard = requestJson["idCard"];

        // 检查用户名是否已存在
        Mapper<User> userMapper;
        Example<User> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&User::username, username);

        auto existingUsers = userMapper.selectByExample(example);
        if (!existingUsers.empty()) {
            res.status = 400;
            responseJson = {{"message", "Username already exists"}, {"userId", ""}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 创建新用户
        User newUser;
        newUser.username = username;
        newUser.password = password; // 密码需加密存储
        newUser.email = email;
        newUser.phone = phone;
        newUser.age = age;
        newUser.gender = gender;
        newUser.idNumber = idCard;

        int userId = userMapper.insert(newUser);
        if (userId <= 0) {
            res.status = 500;
            responseJson = {{"message", "Failed to register user"}, {"userId", ""}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        res.status = 201;
        responseJson = {{"message", "User registered successfully"}, {"userId", userId}};
        res.set_content(responseJson.dump(), "application/json");
    } catch (const json::exception& e) {
        res.status = 400;
        responseJson = {{"message", "Invalid JSON format"}, {"userId", ""}};
        res.set_content(responseJson.dump(), "application/json");
    }
}

int main() {
    Server svr;

    // 登录接口
    svr.Post("/api/login", handle_login);

    // 注册接口
    svr.Post("/api/register", handle_register);

    cout << "Server is running on http://localhost:8080" << endl;
    svr.listen("localhost", 8080);

    return 0;
}
