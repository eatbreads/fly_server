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
                {"phone", user.phone},
                {"age", user.age},
                {"gender", user.gender},
                {"idNumber", user.idNumber},
                {"userLevel", user.userLevel}
            }}
        };
        res.set_content(responseJson.dump(), "application/json");
    } catch (const json::exception& e) {
        res.status = 200;
        responseJson = {{"code", 500}, {"message", "Login failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}

// 注销用户接口
void handle_logout(const Request& req, Response& res) {
    json responseJson;

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("email")) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Logout failed: Missing required fields"}, {"data", nullptr}}; 
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        string email = requestJson["email"];

        // 检查邮箱是否已存在
        Mapper<User> userMapper;
        Example<User> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&User::email, email);

        auto users = userMapper.selectByExample(example);

        
        if (users.empty()) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Logout failed: User not found"}, {"data", nullptr}}; 
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 这里的注销操作可以是清除登录状态、删除 Token 等，这里暂时不做具体实现
        res.status = 200;
        responseJson = {{"code", 200}, {"message", "Logout successful"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");

    } catch (const json::exception& e) {
        res.status = 200;
        responseJson = {{"code", 500}, {"message", "Logout failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}


// 更改用户信息接口
void handle_update_user_info(const Request& req, Response& res) {
    json responseJson;

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("id")) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Update failed: Missing user ID"}, {"data", nullptr}}; 
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

       int userId = requestJson["id"];
        string username = requestJson.value("username", "");
        string email = requestJson.value("email", "");
        string phone = requestJson.value("phone", "");
        int age = requestJson.value("age", 0);
        string gender = requestJson.value("gender", "");
        string idNumber = requestJson.value("idNumber", "");
        int userLevel = requestJson.value("userLevel", 0);

        // 使用 ORM 查询用户
        Mapper<User> userMapper;
        Example<User> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&User::email, email);

        auto users = userMapper.selectByExample(example);

        if (users.empty()) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "User not found"}, {"data", nullptr}}; 
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        User& user = users[0]; // 获取找到的用户对象

        // 更新用户信息
        if (!username.empty()) user.username = username;
        if (!email.empty()) user.email = email;
        if (!phone.empty()) user.phone = phone;
        if (age > 0) user.age = age;
        if (!gender.empty()) user.gender = gender;
        if (!idNumber.empty()) user.idNumber = idNumber;
        if (userLevel >= 0) user.userLevel = userLevel;

        int updatedRows = userMapper.updateByPrimaryKey(user);
        if (updatedRows > 0) {
            res.status = 200;
            responseJson = {{"code", 200}, {"message", "Update successful"}, {"data", {{"id", userId}}}}; 
        } else {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Update failed: Database error"}, {"data", nullptr}}; 
        }

        res.set_content(responseJson.dump(), "application/json");
    } catch (const json::exception& e) {
        res.status = 200;
        responseJson = {{"code", 500}, {"message", "Update failed: Invalid JSON format"}, {"data", nullptr}}; 
        res.set_content(responseJson.dump(), "application/json");
    }
}


// 根据 email 获取用户信息接口
void handle_get_user_by_id(const Request& req, Response& res) {
    json responseJson;

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);
        // 解析请求参数
        if (!requestJson.contains("email")) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Get user failed: Missing user email"}, {"data", nullptr}}; 
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        string email = requestJson.value("email", "");

        // 使用 ORM 查询用户
        Mapper<User> userMapper;
        Example<User> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&User::email, email);

        auto users = userMapper.selectByExample(example);

        if (users.empty()) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "User not found"}, {"data", nullptr}}; 
        } else {
            User& user = users[0]; // 获取找到的用户对象

            res.status = 200;
            responseJson = {{"code", 200},
                            {"message", "User retrieved successfully"},
                            {"data", {{"id", user.id},
                                      {"username", user.username},
                                      {"email", user.email},
                                      {"phone", user.phone},
                                      {"age", user.age},
                                      {"gender", user.gender},
                                      {"idNumber", user.idNumber},
                                      {"userLevel", user.userLevel}}}}; 
        }

        res.set_content(responseJson.dump(), "application/json");
    } catch (const std::exception& e) {
        res.status = 200;
        responseJson = {{"code", 500}, {"message", "Get user failed: Invalid parameter"}, {"data", nullptr}}; 
        res.set_content(responseJson.dump(), "application/json");
    }
}
