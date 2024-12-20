//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_CRITERION_HPP
#define MAPPER_CRITERION_HPP

#include <string>
#include "SQLConstants.hpp"
#include "Object.hpp"

/**
 * 通用的一个标准
 */
class Criterion {
private:
    std::string condition;
    std::vector<Object> values;//传入的比较值,之后可以通过实体字段类型获取,一般between会有第二个值

    std::string andOr;//and和or的连接语句
    bool noValue = false;//是否是无值的
    bool singleValue = false;//是否是单值
    bool betweenValue = false;//是否是between
    bool listValue = false;//是否是列表值

public:
    const std::string &getCondition() const {
        return condition;
    }

    const std::string &getAndOr() const {
        return andOr;
    }

    bool isNoValue() const {
        return noValue;
    }

    bool isSingleValue() const {
        return singleValue;
    }

    bool isBetweenValue() const {
        return betweenValue;
    }

    bool isListValue() const {
        return listValue;
    }

    const std::vector<Object> &getValues() const {
        return values;
    }

public:

    Criterion(const std::string &condition, const Object &value, const Object &secondValue, bool isOr = false) {
        this->condition = condition;
        this->values.emplace_back(value);
        this->values.emplace_back(secondValue);
        this->betweenValue = true;
        this->andOr = isOr ? SQLConstants::OR : SQLConstants::AND;
    }

    Criterion(const std::string &condition, const Object &value, bool isOr = false) {
        this->condition = condition;
        this->andOr = isOr ? SQLConstants::OR : SQLConstants::AND;
        if (value.isContainer()) {
            this->listValue = true;
            //一个个插入
            this->values.insert(this->values.end(), value.toVector().begin(), value.toVector().end());
        } else {
            this->singleValue = true;
            this->values.emplace_back(value);
        }
    }

    explicit Criterion(const std::string &condition, bool isOr = false) {
        this->condition = condition;
        this->noValue = true;
        this->andOr = isOr ? SQLConstants::OR : SQLConstants::AND;
    }
};

#endif //MAPPER_CRITERION_HPP
