#pragma once

#include <utility/httplib.h>          // 新的 Web 库
#include <iostream>
#include <utility/json.hpp>           // 新的 JSON 库
#include <common/Mapper.hpp>          // ORM 库
#include <entity.hpp>                 // 包含 User、Order、Flight 类的定义
#include <utility/jwt.h>              // JWT 工具

using namespace httplib;
using namespace std;
using json = nlohmann::json;

// 订单创建接口
void handle_order_create(const Request& req, Response& res) {
    json responseJson;
    try {
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("userId") || 
            !requestJson.contains("flightId") || 
            !requestJson.contains("seatType") || 
            !requestJson.contains("price")) {
            responseJson = {{"code", 500}, {"message", "Order creation failed: Missing required fields"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        int userId = requestJson["userId"];
        int flightId = requestJson["flightId"];
        string seatType = requestJson["seatType"];
        int price = requestJson["price"];

        Mapper<Flight> flightMapper;
        Example<Flight> flightExample;
        auto flightCriteria = flightExample.createCriteria();
        flightCriteria->andEqualTo(&Flight::id, flightId);

        auto flights = flightMapper.selectByExample(flightExample);

        // 检查航班是否存在
        if (flights.empty()) {
            responseJson = {{"code", 500}, {"message", "Order creation failed: Flight not found"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        auto& flight = flights[0];

        // 检查座位数量
        int& availableSeats = (seatType == "Economy") ? flight.economyClassSeats : flight.firstClassSeats;
        if (availableSeats <= 0) {
            responseJson = {{"code", 500}, {"message", "Order creation failed: Flight fully booked"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 减少座位数量并更新
        availableSeats--;
        flightMapper.updateByPrimaryKey(flight);

        // 创建订单
        Mapper<Order> orderMapper;
        Order newOrder;
        newOrder.userId = userId;
        newOrder.flightId = flightId;
        newOrder.seatType = seatType;
        newOrder.price = price;
        newOrder.orderStatus = "Pending payment";

        int orderId = orderMapper.insert(newOrder);
        if (orderId > 0) {
            responseJson = {
                {"code", 200},
                {"message", "Order created successfully"},
                {"data", {{"id", orderId}, {"status", "Pending payment"}}}
            };
        } else {
            responseJson = {{"code", 500}, {"message", "Order creation failed: Database error"}, {"data", nullptr}};
        }
        res.set_content(responseJson.dump(), "application/json");

    } catch (const json::exception& e) {
        responseJson = {{"code", 500}, {"message", "Order creation failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}

// 查询订单接口
void handle_order_search(const Request& req, Response& res) {
    json responseJson;
    try {
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("userId")) {
            responseJson = {{"code", 500}, {"message", "Query failed: Missing userId"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        int userId = requestJson["userId"];

        Mapper<Order> orderMapper;
        Example<Order> orderExample;
        auto orderCriteria = orderExample.createCriteria();
        orderCriteria->andEqualTo(&Order::userId, userId);

        auto orders = orderMapper.selectByExample(orderExample);

        if (orders.empty()) {
            responseJson = {{"code", 500}, {"message", "Query failed: No orders found"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 构造响应数据
        std::vector<json> orderData;
        Mapper<Flight> flightMapper;
        for (const auto& order : orders) {
            Example<Flight> flightExample;
            auto flightCriteria = flightExample.createCriteria();
            flightCriteria->andEqualTo(&Flight::id, order.flightId);

            auto flights = flightMapper.selectByExample(flightExample);
            if (!flights.empty()) {
                auto& flight = flights[0];
                orderData.push_back({
                    {"id", order.id},
                    {"flightNumber", flight.flightNumber},
                    {"seatType", order.seatType},
                    {"price", order.price},
                    {"status", order.orderStatus},
                    {"paymentTime", order.paymentTime},
                    {"refundTime", order.refundTime}
                });
            }
        }

        responseJson = {
            {"code", 200},
            {"message", "Query successful"},
            {"data", orderData}
        };
        res.set_content(responseJson.dump(), "application/json");

    } catch (const json::exception& e) {
        responseJson = {{"code", 500}, {"message", "Query failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}

// 更新订单接口
void handle_order_update(const Request& req, Response& res) {
    json responseJson;
    try {
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("orderId") || !requestJson.contains("status")) {
            responseJson = {{"code", 500}, {"message", "Update failed: Missing required fields"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        int orderId = requestJson["orderId"];
        string status = requestJson["status"];
        time_t paymentTime = requestJson.value("paymentTime", 0);

        Mapper<Order> orderMapper;
        Example<Order> orderExample;
        auto orderCriteria = orderExample.createCriteria();
        orderCriteria->andEqualTo(&Order::id, orderId);

        auto orders = orderMapper.selectByExample(orderExample);
        if (orders.empty()) {
            responseJson = {{"code", 500}, {"message", "Order update failed: Invalid order ID"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        auto& order = orders[0];
        order.orderStatus = status;
        if (status == "Paid") {
            order.paymentTime = paymentTime;
        } else if (status == "Refunded") {
            order.refundTime = paymentTime;
        }

        orderMapper.updateByPrimaryKey(order);

        responseJson = {
            {"code", 200},
            {"message", "Order updated successfully"},
            {"data", {{"orderId", orderId}, {"status", status}}}
        };
        res.set_content(responseJson.dump(), "application/json");

    } catch (const json::exception& e) {
        responseJson = {{"code", 500}, {"message", "Update failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}
