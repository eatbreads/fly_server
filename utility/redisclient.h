#ifndef REDIS_CLIENT_H
#define REDIS_CLIENT_H

#include <string>
#include <unordered_map>
#include <vector>
#include <hiredis/hiredis.h>

class RedisClient {
public:
    RedisClient(const std::string &host, int port);
    ~RedisClient();

    // 连接 Redis 服务器
    bool connect();
    void disconnect();

    // 键值操作
    bool set(const std::string &key, const std::string &value);
    std::string get(const std::string &key);
    bool del(const std::string &key);
    bool exists(const std::string &key);
    bool expire(const std::string &key, int seconds);

    // 哈希操作
    bool hset(const std::string &key, const std::string &field, const std::string &value);
    std::string hget(const std::string &key, const std::string &field);
    std::unordered_map<std::string, std::string> hgetall(const std::string &key);
    bool hdel(const std::string &key, const std::string &field);

    // 列表操作
    bool lpush(const std::string &key, const std::string &value);
    bool rpush(const std::string &key, const std::string &value);
    std::vector<std::string> lrange(const std::string &key, int start, int end);
    std::string lpop(const std::string &key);
    std::string rpop(const std::string &key);

private:
    std::string m_host;
    int m_port;
    redisContext *m_context; // Redis 连接上下文

    redisReply *executeCommand(const std::string &command);
};

#endif // REDIS_CLIENT_H

// int main() {
//     RedisClient client("127.0.0.1", 6379);
//     if (!client.connect()) {
//         std::cerr << "Failed to connect to Redis!" << std::endl;
//         return 1;
//     }

//     // 键值操作
//     client.set("name", "Alice");
//     std::cout << "Name: " << client.get("name") << std::endl;

//     // 哈希操作
//     client.hset("user:1", "age", "30");
//     std::cout << "Age: " << client.hget("user:1", "age") << std::endl;

//     // 列表操作
//     client.lpush("tasks", "task1");
//     client.lpush("tasks", "task2");
//     for (const auto &task : client.lrange("tasks", 0, -1)) {
//         std::cout << "Task: " << task << std::endl;
//     }

//     client.disconnect();
//     return 0;
// }