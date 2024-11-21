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


#endif //MAPPER_TEST_HPP
