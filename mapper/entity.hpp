//
// Created by Jsiyong on 2020-05-23.
//

#ifndef MAPPER_TEST_HPP
#define MAPPER_TEST_HPP

#include <ctime>
#include <string>
#include <EntityWrapper.hpp>
#include <ostream>

struct Class {
    int classId = 0;
    std::string className;
    int schoolId = 0; 

    friend std::ostream &operator<<(std::ostream &os, const Class &aClass) {
        os << "classId: " << aClass.classId << " className: " << aClass.className << " schoolId: " << aClass.schoolId;
        return os;
    }
};

ResultMap(
        EntityMap(Class), 
        PropertyMap(classId, ColumnType::Id),
        PropertyMap(className),
        PropertyMap(schoolId)
)

struct School {
    int id = 0;
    std::string name;
    std::time_t createTime = {};

    School() = default;

    School(const std::string &name, time_t createTime) : name(name), createTime(createTime) {}

    School(int id, const std::string &name, time_t createTime) : id(id), name(name), createTime(createTime) {}
    
    friend std::ostream &operator<<(std::ostream &os, const School &school) {
        os << "id: " << school.id << " name: " << school.name << " createTime: " << school.createTime;
        return os;
    }

};

ResultMap(
        EntityMap(School, "t_school"),
        PropertyMap(id, ColumnType::Id),
        PropertyMap(name),
        PropertyMap(createTime)
)

struct Student {
    int id = 0;
    std::string name;
    Class clazz;
    std::time_t createTime;

    friend std::ostream &operator<<(std::ostream &os, const Student &student) {
        os << "id: " << student.id << " name: " << student.name << " clazz: " << student.clazz << " createTime: "
           << student.createTime;
        return os;
    }
};

ResultMap(
        EntityMap(Student),
        PropertyMap(id, ColumnType::Id),
        PropertyMap(name),
        PropertyMap(clazz, "class_id", JoinType::OneToOne, &Class::classId),
        PropertyMap(createTime)
)


struct User {  
    int id = 0;                        // 主键 ID  
    std::string username;              // 用户名  
    std::string password;              // 密码  
    std::string email;                 // 邮箱  
    std::string phone;                 // 手机号  
    int age = 0;                       // 年龄  
    std::string gender;                 // 性别 (M: 男, F: 女)  
    std::string idNumber;              // 身份证号  
    int userLevel = 0;                 // 用户等级  

    // 默认构造函数
    User() = default;

    // 带部分字段的构造函数
    User(const std::string &username, const std::string &password, const std::string &email, const std::string &phone, 
         int age, std::string gender, const std::string &idNumber, int userLevel) 
        : username(username), password(password), email(email), phone(phone), 
          age(age), gender(gender), idNumber(idNumber), userLevel(userLevel) {}

    // 带 ID 的完整构造函数
    User(int id, const std::string &username, const std::string &password, const std::string &email, const std::string &phone, 
         int age, std::string gender, const std::string &idNumber, int userLevel) 
        : id(id), username(username), password(password), email(email), phone(phone), 
          age(age), gender(gender), idNumber(idNumber), userLevel(userLevel) {}

    // 输出操作符重载
    friend std::ostream& operator<<(std::ostream& os, const User& user) {  
        os << "id: " << user.id  
           << " username: " << user.username  
           << " password: " << user.password  
           << " email: " << user.email  
           << " phone: " << user.phone  
           << " age: " << user.age  
           << " gender: " << user.gender  
           << " idNumber: " << user.idNumber  
           << " userLevel: " << user.userLevel;  
        return os;  
    }  
};  

// 映射规则  
ResultMap(  
    EntityMap(User, "users"),  
    PropertyMap(id, ColumnType::Id),               // 主键映射  
    PropertyMap(username),                         // 映射到 username 字段  
    PropertyMap(password),                         // 映射到 password 字段  
    PropertyMap(email),                            // 映射到 email 字段  
    PropertyMap(phone),                            // 映射到 phone 字段  
    PropertyMap(age),                              // 映射到 age 字段  
    PropertyMap(gender),                           // 映射到 gender 字段  
    PropertyMap(idNumber, "id_number"),            // 映射到 id_number 字段  
    PropertyMap(userLevel, "user_level")           // 映射到 user_level 字段  
);  
struct Flight {
    int id = 0;                              // 主键 ID
    std::string departure;                   // 出发地
    std::time_t departureTime;               // 出发时间
    std::string destination;                 // 目的地
    std::time_t arrivalTime;                 // 预计到达时间
    std::string flightNumber;                // 航班号
    int firstClassSeats = 0;                 // 头等舱余量
    int businessClassSeats = 0;              // 商务舱余量
    int economyClassSeats = 0;               // 经济舱余量
    int firstClassPrice = 0;            // 头等舱票价
    int businessClassPrice = 0;         // 商务舱票价
    int economyClassPrice = 0;          // 经济舱票价
    std::string departureAirport;            // 出发机场名
    std::string arrivalAirport;              // 到达机场名
    std::string boardingGate;                // 进站口
    std::string aircraftModel;               // 飞机型号
    std::string airlineCompany;              // 航空公司
    int hasMeal = 0;                    // 有无餐食
    int luggageSizeLimit = 0;           // 行李大小限制
    int isInternational = 0;            // 是否国际航班

    friend std::ostream& operator<<(std::ostream& os, const Flight& flight) {
        os << "id: " << flight.id
           << " departure: " << flight.departure
           << " departureTime: " << flight.departureTime
           << " destination: " << flight.destination
           << " arrivalTime: " << flight.arrivalTime
           << " flightNumber: " << flight.flightNumber
           << " firstClassSeats: " << flight.firstClassSeats
           << " businessClassSeats: " << flight.businessClassSeats
           << " economyClassSeats: " << flight.economyClassSeats
           << " firstClassPrice: " << flight.firstClassPrice
           << " businessClassPrice: " << flight.businessClassPrice
           << " economyClassPrice: " << flight.economyClassPrice
           << " departureAirport: " << flight.departureAirport
           << " arrivalAirport: " << flight.arrivalAirport
           << " boardingGate: " << flight.boardingGate
           << " aircraftModel: " << flight.aircraftModel
           << " airlineCompany: " << flight.airlineCompany
           << " hasMeal: " << (flight.hasMeal ? "Yes" : "No")
           << " luggageSizeLimit: " << flight.luggageSizeLimit
           << " isInternational: " << (flight.isInternational ? "Yes" : "No");
        return os;
    }
};

ResultMap(
    EntityMap(Flight, "flights"),
    PropertyMap(id, ColumnType::Id),
    PropertyMap(departure),
    PropertyMap(departureTime, "departure_time"),
    PropertyMap(destination),
    PropertyMap(arrivalTime, "arrival_time"),
    PropertyMap(flightNumber, "flight_number"),
    PropertyMap(firstClassSeats, "first_class_seats"),
    PropertyMap(businessClassSeats, "business_class_seats"),
    PropertyMap(economyClassSeats, "economy_class_seats"),
    PropertyMap(firstClassPrice, "first_class_price"),
    PropertyMap(businessClassPrice, "business_class_price"),
    PropertyMap(economyClassPrice, "economy_class_price"),
    PropertyMap(departureAirport, "departure_airport"),
    PropertyMap(arrivalAirport, "arrival_airport"),
    PropertyMap(boardingGate, "boarding_gate"),
    PropertyMap(aircraftModel, "aircraft_model"),
    PropertyMap(airlineCompany, "airline_company"),
    PropertyMap(hasMeal, "has_meal"),
    PropertyMap(luggageSizeLimit, "luggage_size_limit"),
    PropertyMap(isInternational, "is_international")
);
struct Order {
    int id = 0;                              // 主键 ID
    int userId = 0;                          // 用户 ID
    int flightId = 0;                        // 航班信息 ID
    std::string seatType;                    // 座位类别
    int price = 0;                           // 票价
    std::string orderStatus;                 // 订单状态
    std::time_t paymentTime = {};            // 支付时间
    std::time_t refundTime = {};             // 退款时间

    friend std::ostream& operator<<(std::ostream& os, const Order& order) {
        os << "id: " << order.id
           << " userId: " << order.userId
           << " flightId: " << order.flightId
           << " seatType: " << order.seatType
           << " price: " << order.price
           << " orderStatus: " << order.orderStatus
           << " paymentTime: " << order.paymentTime
           << " refundTime: " << order.refundTime;
        return os;
    }
};

ResultMap(
    EntityMap(Order, "orders"),
    PropertyMap(id, ColumnType::Id),
    PropertyMap(userId, "user_id"),          // 映射到 user_id 字段
    PropertyMap(flightId, "flight_id"),      // 映射到 flight_id 字段
    PropertyMap(seatType, "seat_type"),
    PropertyMap(price),
    PropertyMap(orderStatus, "order_status"),
    PropertyMap(paymentTime, "payment_time"),
    PropertyMap(refundTime, "refund_time")
)



#endif //MAPPER_TEST_HPP