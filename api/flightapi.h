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

// 查询航班信息接口
void handle_flight_search(const Request& req, Response& res) {
    json responseJson;
    try {
        // 解析 JSON 请求体

        json requestJson = json::parse(req.body);
         

        // 检查必要字段：departure 和 destination
        if (!requestJson.contains("departure") || !requestJson.contains("destination")) {
            responseJson = {
                {"code", 500},
                {"message", "Search failed: Missing required fields"},
                {"data", nullptr}
            };
            res.set_content(responseJson.dump(), "application/json");
            return;
        }
        std::string departure = requestJson["departure"];
        std::string destination = requestJson["destination"];

        // 获取当前时间，过滤过期航班
        std::time_t currentTime = std::time(nullptr);

        // 使用 ORM 查询航班信息
        Mapper<Flight> flightMapper;

        Example<Flight> example;

        auto criteria = example.createCriteria();


        criteria->andEqualTo(&Flight::departure, departure);

        criteria->andEqualTo(&Flight::destination, destination);

        //criteria->andGreaterThanOrEqualTo(&Flight::departureTime, currentTime); // 过滤过期航班
        criteria->andGreaterThan(&Flight::departureTime, currentTime); // 过滤过期航班

        auto flights = flightMapper.selectByExample(example);

        // 检查是否找到符合条件的航班
        if (flights.empty()) {
            responseJson = {
                {"code", 500},
                {"message", "Search failed: No flights found"},
                {"data", nullptr}
            };
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 构造响应数据
        std::vector<json> flightData;
        for (const auto& flight : flights) {
            flightData.push_back({
                {"id", flight.id},
                {"flightNumber", flight.flightNumber},
                {"departure", flight.departure},
                {"destination", flight.destination},
                {"departureTime", flight.departureTime},
                {"arrivalTime", flight.arrivalTime},
                {"firstClassSeats", flight.firstClassSeats},
                {"economyClassSeats", flight.economyClassSeats},
                {"firstClassPrice", flight.firstClassPrice},
                {"economyClassPrice", flight.economyClassPrice},
                {"airlineCompany", flight.airlineCompany}
            });
        
        responseJson = {
            {"code", 200},
            {"message", "Search successful"},
            {"data", flightData}
        };
        res.set_content(responseJson.dump(), "application/json");

    } }catch (const json::exception& e) {
        responseJson = {
            {"code", 500},
            {"message", "Search failed: Invalid JSON format"},
            {"data", nullptr}
        };
        res.set_content(responseJson.dump(), "application/json");
    } catch (...) {
        responseJson = {
            {"code", 500},
            // {"message", "Search failed: Internal server error"},
            {"message", i},
            {"data", nullptr}
        };
        res.set_content(responseJson.dump(), "application/json");
    }
}


// 创建航班信息接口
void handle_flight_create(const Request& req, Response& res) {
    json responseJson;

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);

        // 检查必要字段
        if (!requestJson.contains("flightNumber") || 
            !requestJson.contains("departure") || 
            !requestJson.contains("destination") || 
            !requestJson.contains("departureTime") || 
            !requestJson.contains("arrivalTime") || 
            !requestJson.contains("firstClassSeats") || 
            !requestJson.contains("economyClassSeats") || 
            !requestJson.contains("firstClassPrice") || 
            !requestJson.contains("economyClassPrice") || 
            !requestJson.contains("airlineCompany") || 
            !requestJson.contains("aircraftModel")) {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Flight creation failed: Missing required fields"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        std::string flightNumber = requestJson["flightNumber"];

        // 检查航班号是否重复
        Mapper<Flight> flightMapper;
        Example<Flight> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&Flight::flightNumber, flightNumber);

        auto existingFlights = flightMapper.selectByExample(example);
        if (!existingFlights.empty()) {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Flight creation failed: Duplicate flight number"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 创建新航班
        Flight newFlight;
        newFlight.flightNumber = flightNumber;
        newFlight.departure = requestJson["departure"];
        newFlight.destination = requestJson["destination"];
        newFlight.departureTime = requestJson["departureTime"];
        newFlight.arrivalTime = requestJson["arrivalTime"];
        newFlight.firstClassSeats = requestJson["firstClassSeats"];
        newFlight.economyClassSeats = requestJson["economyClassSeats"];
        newFlight.firstClassPrice = requestJson["firstClassPrice"];
        newFlight.economyClassPrice = requestJson["economyClassPrice"];
        newFlight.airlineCompany = requestJson["airlineCompany"];
        newFlight.aircraftModel = requestJson["aircraftModel"];

        // 插入航班
        int flightId = flightMapper.insert(newFlight);
        if (flightId > 0) {
            res.status = 200;
            responseJson = {
                {"code", 200},
                {"message", "Flight created successfully"},
                {"data", {{"id", flightId}, {"flightNumber", flightNumber}}}
            };
        } else {
            res.status = 200;
            responseJson = {{"code", 500}, {"message", "Flight creation failed: Database error"}, {"data", nullptr}};
        }
        res.set_content(responseJson.dump(), "application/json");
    } catch (const json::exception& e) {
        res.status = 200;
        responseJson = {{"code", 500}, {"message", "Flight creation failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}
