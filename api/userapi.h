#pragma once

#include <utility/httplib.h>          // 新的 Web 库
#include <iostream>
#include <utility/json.hpp> // 新的 JSON 库
#include <common/Mapper.hpp>  // ORM 库
#include <entity.hpp>           // 包含 User 类的定义
#include <utility/jwt.h>      // JWT 工具

using namespace httplib;
using namespace std;
using json = nlohmann::json;



// 用户注册接口
void handle_register(const Request& req, Response& res) {
    json responseJson;

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("username") || !requestJson.contains("password") ||
            !requestJson.contains("email") || !requestJson.contains("phone") ||
            !requestJson.contains("age") || !requestJson.contains("gender") ||
            !requestJson.contains("idNumber") || !requestJson.contains("userLevel")) {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Registration failed: Missing required fields"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        string username = requestJson["username"];
        string email = requestJson["email"];

        // 检查邮箱是否已存在
        Mapper<User> userMapper;
        Example<User> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&User::email, email);

        auto existingUsers = userMapper.selectByExample(example);
        if (!existingUsers.empty()) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Registration failed: Email already exists"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 创建新用户
        User newUser;
        newUser.username = username;
        newUser.password = requestJson["password"];
        newUser.email = email;
        newUser.phone = requestJson["phone"];
        newUser.age = requestJson["age"];
        newUser.gender = requestJson["gender"];
        newUser.idNumber = requestJson["idNumber"];
        newUser.userLevel = requestJson["userLevel"];

        // 插入用户
        int userId = userMapper.insert(newUser);
        if (userId > 0) {
            res.status = 200;
            responseJson = {
                {"code", 200},
                {"message", "Registration successful"},
                {"data", {{"id", userId}, {"username", username}, {"email", email}}}
            };
        } else {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Registration failed: Database error"}, {"data", nullptr}};
        }
        res.set_content(responseJson.dump(), "application/json");
    } catch (const json::exception& e) {
        res.status = 200;
        responseJson = {{"code", 500}, {"message", "Registration failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}

// 用户登录接口
void handle_login(const Request& req, Response& res) {
    json responseJson;

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("email") || !requestJson.contains("password")) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Login failed: Missing required fields"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        string email = requestJson["email"];
        string password = requestJson["password"];

        // 使用 ORM 查询用户
        Mapper<User> userMapper;
        Example<User> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&User::email, email);

        auto users = userMapper.selectByExample(example);

        if (users.empty() || users[0].password != password) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Login failed: Invalid email or password"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        User user = users[0];
        res.status = 200;
        responseJson = {
            {"code", 200},
            {"message", "Login successful"},
            {"data", {
                {"id", user.id},
                {"username", user.username},
                {"email", user.email},
                {"role", "学生"} // 假定角色固定为"学生"，根据需求修改
            }}
        };
        res.set_content(responseJson.dump(), "application/json");
    } catch (const json::exception& e) {
        res.status = 200;
        responseJson = {{"code", 500}, {"message", "Login failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}
