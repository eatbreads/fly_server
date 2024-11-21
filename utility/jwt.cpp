#include "utility/jwt.h"
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <sstream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <iostream>
using namespace std::chrono;
JWT::JWT(const std::string &secret) : m_secret(secret) {}

std::string JWT::generate(const std::string &header, const std::string &payload, int expiry_seconds) {
    auto now = std::chrono::system_clock::now();
    auto now_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
    long exp_time = now_seconds + expiry_seconds;

    // 在 payload 中拼接 iat 和 exp 信息
    std::string extendedPayload = payload +
                                  "|iat:" + std::to_string(now_seconds) +
                                  "|exp:" + std::to_string(exp_time);

    std::string encodedHeader = base64UrlEncode(header);
    std::string encodedPayload = base64UrlEncode(extendedPayload);
    std::cout << "extended payload: " << extendedPayload << std::endl;
    std::cout << "encode  payload: " << encodedPayload << std::endl;


    std::string dataToSign = encodedHeader + "." + encodedPayload;

    std::string signature = hmacSha256(m_secret, dataToSign);
    std::string encodedSignature = base64UrlEncode(signature);

    return encodedHeader + "." + encodedPayload + "." + encodedSignature;
}

bool JWT::verify(const std::string &token) const {
    auto parts = splitToken(token);
    if (parts.size() != 3) return false;

    std::string dataToVerify = parts[0] + "." + parts[1];
    std::string expectedSignature = base64UrlEncode(hmacSha256(m_secret, dataToVerify));

    return expectedSignature == parts[2];
}

std::string JWT::getPayload(const std::string &token) const {
    auto parts = splitToken(token);
    if (parts.size() != 3) throw std::invalid_argument("Invalid JWT format");

    return base64UrlDecode(parts[1]);
}

bool JWT::isExpired(const std::string &token) const {
    std::string payload = getPayload(token);
    std::cout <<"payload:"<< payload << std::endl;
    long exp_time = extractField(payload, "exp");

    auto now = std::chrono::system_clock::now();
    auto now_seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();

    return now_seconds > exp_time;
}

std::string JWT::base64UrlEncode(const std::string &input) const {
    BIO *bio, *b64;
    BUF_MEM *bufferPtr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // 禁止换行
    bio = BIO_new(BIO_s_mem());
    bio = BIO_push(b64, bio);

    BIO_write(bio, input.data(), input.size());
    BIO_flush(bio); // 确保写入完整数据
    BIO_get_mem_ptr(bio, &bufferPtr);

    // 完整读取缓冲区数据
    std::string result(bufferPtr->data, bufferPtr->length);
    BIO_free_all(bio);

    // 替换为 URL 安全的字符
    std::replace(result.begin(), result.end(), '+', '-');
    std::replace(result.begin(), result.end(), '/', '_');
    result.erase(std::remove(result.begin(), result.end(), '='), result.end());

    return result;
}

std::string JWT::base64UrlDecode(const std::string &input) const {
    std::string base64 = input;
    std::replace(base64.begin(), base64.end(), '-', '+');
    std::replace(base64.begin(), base64.end(), '_', '/');

    // 补齐 Base64 编码到 4 的倍数长度
    while (base64.size() % 4 != 0) {
        base64 += '=';
    }

    BIO *bio, *b64;
    char buffer[1024];
    std::string output;

    bio = BIO_new_mem_buf(base64.data(), base64.size());
    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL); // 禁止换行
    bio = BIO_push(b64, bio);

    int len = BIO_read(bio, buffer, sizeof(buffer));
    if (len > 0) {
        output.assign(buffer, len);
    }
    BIO_free_all(bio);

    return output;
}


std::string JWT::hmacSha256(const std::string &key, const std::string &data) const{
    unsigned char result[EVP_MAX_MD_SIZE];
    unsigned int len = 0;

    HMAC_CTX ctx;
    HMAC_CTX_init(&ctx);

    HMAC_Init_ex(&ctx, key.c_str(), key.length(), EVP_sha256(), NULL);
    HMAC_Update(&ctx, (unsigned char *)data.c_str(), data.length());
    HMAC_Final(&ctx, result, &len);

    HMAC_CTX_cleanup(&ctx);

    return std::string(reinterpret_cast<char *>(result), len);
}

std::vector<std::string> JWT::splitToken(const std::string &token) const {
    std::vector<std::string> parts;
    std::stringstream ss(token);
    std::string part;
    while (std::getline(ss, part, '.')) {
        parts.push_back(part);
    }
    return parts;
}

long JWT::extractField(const std::string &payload, const std::string &field) const {
    size_t pos = payload.find("|" + field + ":");
    if (pos == std::string::npos) {
        std::cerr << "Field not found: " << field << " in payload: " << payload << std::endl;
        throw std::runtime_error("Field not found: " + field);
    }

    size_t start = pos + field.size() + 2;
    size_t end = payload.find("|", start);
    if (end == std::string::npos) end = payload.size();

    try {
        return std::stol(payload.substr(start, end - start));
    } catch (const std::exception &e) {
        std::cerr << "Error parsing field " << field << ": " << e.what() << std::endl;
        throw;
    }
}

