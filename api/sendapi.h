#pragma once
#include <utility/httplib.h>
#include <iostream>
#include <utility/json.hpp>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <memory> 

using namespace httplib;
using namespace std;
using json = nlohmann::json;

// 调用 Python 脚本发送邮件
// 调用 Python 脚本发送邮件
bool send_email(const string& from_email, const string& to_email, const string& subject, const string& body, const string& auth_code) {
    // 创建 Python 命令，传递给 Python 脚本的参数
    string command = "python3 ../send_email.py \"" + from_email + "\" \"" + to_email + "\" \"" + subject + "\" \"" + body + "\" \"" + auth_code + "\"";

    // 打印命令，调试用
    cout << "Running command: " << command << endl;

    // 使用 popen 调用 Python 脚本并捕获其输出
    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        cerr << "Failed to run Python script" << endl;
        return false;
    }

    // 读取 Python 脚本的输出
    char buffer[128];
    string result = "";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        result += buffer;
    }

    // 获取脚本的返回状态
    int returnCode = pclose(pipe);
    
    // 判断 Python 脚本的返回值是否为0，来决定是否成功
    if (returnCode == 0) {
        // 判断 Python 脚本的输出是否包含“Email sent successfully”
        if (result.find("Email sent successfully") != string::npos) {
            return true;
        }
    }

    // 如果返回值不是0，或者脚本输出不是成功的标志，则认为失败
    cerr << "Python script error: " << result << endl;
    return false;
}



// 发送邮件的 API 接口
void handle_send_email(const Request& req, Response& res) {
    json responseJson;

    try {
        // 解析请求体
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("to") ||
            !requestJson.contains("subject") || !requestJson.contains("body")) {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Missing required fields (to, subject, body)"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 获取邮件信息
        string to = requestJson["to"];
        string subject = requestJson["subject"];
        string body = requestJson["body"];
        string from = "1852611363@qq.com";  // 默认发件人
        string auth_code = "tlffzoqsuttaedhc";  // 默认授权码

        // 调用 Python 脚本发送邮件
        if (send_email(from, to, subject, body, auth_code)) {
            res.status = 200;
            responseJson = {{"code", 200}, {"message", "Email sent successfully"}, {"data", nullptr}};
        } else {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Email sending failed"}, {"data", nullptr}};
        }

        res.set_content(responseJson.dump(), "application/json");

    } catch (const json::exception& e) {
        res.status = 500;
        responseJson = {{"code", 500}, {"message", "Error processing the request"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}
