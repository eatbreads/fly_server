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


        // 使用 ORM 查询航班信息
        Mapper<Flight> flightMapper; 

        Example<Flight> example;
        auto criteria = example.createCriteria();

        criteria->andEqualTo(&Flight::departure, departure);
        criteria->andEqualTo(&Flight::destination, destination);
        //criteria->andGreaterThan(&Flight::departureTime, currentTime); // 过滤过期航班

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
        }

        responseJson = {
            {"code", 200},
            {"message", "Search successful"},
            {"data", flightData}
        };
        res.set_content(responseJson.dump(), "application/json");

    } catch (const json::exception& e) {
        responseJson = {
            {"code", 500},
            {"message", "Search failed: Invalid JSON format"},
            {"data", nullptr}
        };
        res.set_content(responseJson.dump(), "application/json");
    } catch (const std::exception& e) {
        responseJson = {
            {"code", 500},
            {"message", std::string("Search failed: ") + e.what()},
            {"data", nullptr}
        };
        res.set_content(responseJson.dump(), "application/json");
    } catch (...) {
        responseJson = { 
            {"code", 500},
            {"message", "Search failed: Internal server error"},
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

// 修改航班信息接口
void handle_flight_update(const Request& req, Response& res) {
    json responseJson;

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);

        // 检查是否包含航班ID
        if (!requestJson.contains("id")) {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Flight update failed: Missing flight ID"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        int flightId = requestJson["id"];
        
        // 使用 ORM 查询该航班是否存在
        Mapper<Flight> flightMapper;
        Example<Flight> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&Flight::id, flightId);
        auto flights = flightMapper.selectByExample(example);

        if (flights.empty()) {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Flight update failed: Flight not found"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 找到航班并更新字段
        Flight& flight = flights.front();
        
        if (requestJson.contains("departure")) flight.departure = requestJson["departure"];
        if (requestJson.contains("departureTime")) flight.departureTime = requestJson["departureTime"];
        if (requestJson.contains("destination")) flight.destination = requestJson["destination"];
        if (requestJson.contains("arrivalTime")) flight.arrivalTime = requestJson["arrivalTime"];
        if (requestJson.contains("flightNumber")) flight.flightNumber = requestJson["flightNumber"];
        if (requestJson.contains("firstClassSeats")) flight.firstClassSeats = requestJson["firstClassSeats"];
        if (requestJson.contains("businessClassSeats")) flight.businessClassSeats = requestJson["businessClassSeats"];
        if (requestJson.contains("economyClassSeats")) flight.economyClassSeats = requestJson["economyClassSeats"];
        if (requestJson.contains("firstClassPrice")) flight.firstClassPrice = requestJson["firstClassPrice"];
        if (requestJson.contains("businessClassPrice")) flight.businessClassPrice = requestJson["businessClassPrice"];
        if (requestJson.contains("economyClassPrice")) flight.economyClassPrice = requestJson["economyClassPrice"];
        if (requestJson.contains("departureAirport")) flight.departureAirport = requestJson["departureAirport"];
        if (requestJson.contains("arrivalAirport")) flight.arrivalAirport = requestJson["arrivalAirport"];
        if (requestJson.contains("boardingGate")) flight.boardingGate = requestJson["boardingGate"];
        if (requestJson.contains("aircraftModel")) flight.aircraftModel = requestJson["aircraftModel"];
        if (requestJson.contains("airlineCompany")) flight.airlineCompany = requestJson["airlineCompany"];
        if (requestJson.contains("hasMeal")) flight.hasMeal = requestJson["hasMeal"];
        if (requestJson.contains("luggageSizeLimit")) flight.luggageSizeLimit = requestJson["luggageSizeLimit"];
        if (requestJson.contains("isInternational")) flight.isInternational = requestJson["isInternational"];

        // 更新数据库中的航班信息
        int updatedRows = flightMapper.updateByPrimaryKey(flight);
        
        if (updatedRows > 0) {
            res.status = 200;
            responseJson = {{"code", 200}, {"message", "Flight updated successfully"}, {"data", {{"id", flightId}}}};
        } else {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Flight update failed: Database error"}, {"data", nullptr}};
        }

        res.set_content(responseJson.dump(), "application/json");

    } catch (const json::exception& e) {
        res.status = 500;
        responseJson = {{"code", 500}, {"message", "Flight update failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}

// 删除航班信息接口
void handle_flight_delete(const Request& req, Response& res) {
    json responseJson;

    try {
        // 解析 JSON 请求体
        json requestJson = json::parse(req.body);

        // 检查是否包含航班ID
        if (!requestJson.contains("id")) {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Flight deletion failed: Missing flight ID"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        int flightId = requestJson["id"];
        
        // 使用 ORM 查询该航班是否存在
        Mapper<Flight> flightMapper;
        Example<Flight> example;
        auto criteria = example.createCriteria();
        criteria->andEqualTo(&Flight::id, flightId);
        auto flights = flightMapper.selectByExample(example);

        if (flights.empty()) {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Flight deletion failed: Flight not found"}, {"data", nullptr}};
            res.set_content(responseJson.dump(), "application/json");
            return;
        }

        // 删除航班
        int deletedRows = flightMapper.deleteByPrimaryKey(flightId);
        
        if (deletedRows > 0) {
            res.status = 200;
            responseJson = {{"code", 200}, {"message", "Flight deleted successfully"}, {"data", {{"id", flightId}}}};
        } else {
            res.status = 500;
            responseJson = {{"code", 500}, {"message", "Flight deletion failed: Database error"}, {"data", nullptr}};
        }

        res.set_content(responseJson.dump(), "application/json");

    } catch (const json::exception& e) {
        res.status = 500;
        responseJson = {{"code", 500}, {"message", "Flight deletion failed: Invalid JSON format"}, {"data", nullptr}};
        res.set_content(responseJson.dump(), "application/json");
    }
}
