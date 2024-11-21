#include "utility/redisclient.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

RedisClient::RedisClient(const std::string &host, int port)
    : m_host(host), m_port(port), m_context(nullptr) {}

RedisClient::~RedisClient() {
    disconnect();
}

bool RedisClient::connect() {
    m_context = redisConnect(m_host.c_str(), m_port);
    if (m_context == nullptr || m_context->err) {
        if (m_context) {
            std::cerr << "Connection error: " << m_context->errstr << std::endl;
            redisFree(m_context);
        } else {
            std::cerr << "Connection error: Can't allocate redis context" << std::endl;
        }
        return false;
    }
    return true;
}

void RedisClient::disconnect() {
    if (m_context) {
        redisFree(m_context);
        m_context = nullptr;
    }
}

redisReply *RedisClient::executeCommand(const std::string &command) {
    redisReply *reply = (redisReply *)redisCommand(m_context, command.c_str());
    if (!reply) {
        std::cerr << "Command error: " << m_context->errstr << std::endl;
        return nullptr;
    }
    return reply;
}

// 键值操作
bool RedisClient::set(const std::string &key, const std::string &value) {
    std::ostringstream command;
    command << "SET " << key << " " << value;
    redisReply *reply = executeCommand(command.str());
    bool success = reply && reply->type == REDIS_REPLY_STATUS && std::string(reply->str) == "OK";
    freeReplyObject(reply);
    return success;
}

std::string RedisClient::get(const std::string &key) {
    std::ostringstream command;
    command << "GET " << key;
    redisReply *reply = executeCommand(command.str());
    std::string value = reply && reply->type == REDIS_REPLY_STRING ? reply->str : "";
    freeReplyObject(reply);
    return value;
}

bool RedisClient::del(const std::string &key) {
    std::ostringstream command;
    command << "DEL " << key;
    redisReply *reply = executeCommand(command.str());
    bool success = reply && reply->type == REDIS_REPLY_INTEGER && reply->integer > 0;
    freeReplyObject(reply);
    return success;
}

bool RedisClient::exists(const std::string &key) {
    std::ostringstream command;
    command << "EXISTS " << key;
    redisReply *reply = executeCommand(command.str());
    bool exists = reply && reply->type == REDIS_REPLY_INTEGER && reply->integer > 0;
    freeReplyObject(reply);
    return exists;
}

bool RedisClient::expire(const std::string &key, int seconds) {
    std::ostringstream command;
    command << "EXPIRE " << key << " " << seconds;
    redisReply *reply = executeCommand(command.str());
    bool success = reply && reply->type == REDIS_REPLY_INTEGER && reply->integer == 1;
    freeReplyObject(reply);
    return success;
}

// 哈希操作
bool RedisClient::hset(const std::string &key, const std::string &field, const std::string &value) {
    std::ostringstream command;
    command << "HSET " << key << " " << field << " " << value;
    redisReply *reply = executeCommand(command.str());
    bool success = reply && reply->type == REDIS_REPLY_INTEGER && reply->integer > 0;
    freeReplyObject(reply);
    return success;
}

std::string RedisClient::hget(const std::string &key, const std::string &field) {
    std::ostringstream command;
    command << "HGET " << key << " " << field;
    redisReply *reply = executeCommand(command.str());
    std::string value = reply && reply->type == REDIS_REPLY_STRING ? reply->str : "";
    freeReplyObject(reply);
    return value;
}

std::unordered_map<std::string, std::string> RedisClient::hgetall(const std::string &key) {
    std::ostringstream command;
    command << "HGETALL " << key;
    redisReply *reply = executeCommand(command.str());
    std::unordered_map<std::string, std::string> result;
    if (reply && reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; i += 2) {
            std::string field = reply->element[i]->str;
            std::string value = reply->element[i + 1]->str;
            result[field] = value;
        }
    }
    freeReplyObject(reply);
    return result;
}

bool RedisClient::hdel(const std::string &key, const std::string &field) {
    std::ostringstream command;
    command << "HDEL " << key << " " << field;
    redisReply *reply = executeCommand(command.str());
    bool success = reply && reply->type == REDIS_REPLY_INTEGER && reply->integer > 0;
    freeReplyObject(reply);
    return success;
}

// 列表操作
bool RedisClient::lpush(const std::string &key, const std::string &value) {
    std::ostringstream command;
    command << "LPUSH " << key << " " << value;
    redisReply *reply = executeCommand(command.str());
    bool success = reply && reply->type == REDIS_REPLY_INTEGER;
    freeReplyObject(reply);
    return success;
}

bool RedisClient::rpush(const std::string &key, const std::string &value) {
    std::ostringstream command;
    command << "RPUSH " << key << " " << value;
    redisReply *reply = executeCommand(command.str());
    bool success = reply && reply->type == REDIS_REPLY_INTEGER;
    freeReplyObject(reply);
    return success;
}

std::vector<std::string> RedisClient::lrange(const std::string &key, int start, int end) {
    std::ostringstream command;
    command << "LRANGE " << key << " " << start << " " << end;
    redisReply *reply = executeCommand(command.str());
    std::vector<std::string> result;
    if (reply && reply->type == REDIS_REPLY_ARRAY) {
        for (size_t i = 0; i < reply->elements; ++i) {
            result.push_back(reply->element[i]->str);
        }
    }
    freeReplyObject(reply);
    return result;
}

std::string RedisClient::lpop(const std::string &key) {
    std::ostringstream command;
    command << "LPOP " << key;
    redisReply *reply = executeCommand(command.str());
    std::string value = reply && reply->type == REDIS_REPLY_STRING ? reply->str : "";
    freeReplyObject(reply);
    return value;
}

std::string RedisClient::rpop(const std::string &key) {
    std::ostringstream command;
    command << "RPOP " << key;
    redisReply *reply = executeCommand(command.str());
    std::string value = reply && reply->type == REDIS_REPLY_STRING ? reply->str : "";
    freeReplyObject(reply);
    return value;
}
