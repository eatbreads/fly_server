#include <iostream>
#include <string>
#include <openssl/hmac.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#include <openssl/sha.h>
#include <sstream>
#include <iomanip>
#include <stdexcept>
#include <vector>
#include <chrono>
#include <algorithm>
class JWT {
public:
    explicit JWT(const std::string &secret);

    // 生成 JWT，参数为 Header、Payload 和过期时间（秒）
    std::string generate(const std::string &header, const std::string &payload, int expiry_seconds);

    // 验证 JWT 的签名是否合法
    bool verify(const std::string &token) const;

    // 从 JWT 中提取 Payload
    std::string getPayload(const std::string &token) const;

    // 检查 JWT 是否已过期
    bool isExpired(const std::string &token) const;

private: 
    std::string m_secret;

    // Base64 URL 编码
    std::string base64UrlEncode(const std::string &input) const;

    // Base64 URL 解码
    std::string base64UrlDecode(const std::string &input) const;

    // HMAC SHA256 签名生成
    std::string hmacSha256(const std::string &key, const std::string &data) const;

    // 拆分 JWT 为 Header、Payload 和 Signature
    std::vector<std::string> splitToken(const std::string &token) const;

    // 从 Payload 中提取特定字段的值（如 iat 或 exp）
    long extractField(const std::string &payload, const std::string &field) const;
};


// int main() {
//     JWT jwt("my_secret_key");

//     // 创建 Header 和 Payload
//     std::string header = R"({"alg":"HS256","typ":"JWT"})";
//     std::string payload = R"({"user_id":"12345","role":"admin"})";

//     // 生成 JWT，有效期为 1 小时
//     std::string token = jwt.generate(header, payload, 3600);

//     std::cout << "Generated Token: " << token << std::endl;

//     // 验证 Token
//     if (jwt.verify(token)) {
//         std::cout << "Token signature is valid." << std::endl;

//         if (!jwt.isExpired(token)) {
//             std::cout << "Token is valid and not expired." << std::endl;

//             // 提取并打印 Payload
//             std::string decodedPayload = jwt.getPayload(token);
//             std::cout << "Payload: " << decodedPayload << std::endl;
//         } else {
//             std::cout << "Token has expired." << std::endl;
//         
//     } else {
//         std::cout << "Token signature is invalid." << std::endl;
//     }

//     return 0;
// }