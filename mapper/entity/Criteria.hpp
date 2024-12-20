//
// Created by Jsiyong on 2020-05-10.
//

#ifndef MAPPER_CRITERIA_HPP
#define MAPPER_CRITERIA_HPP

#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <MapperException.hpp>
#include "Criterion.hpp"
#include "EntityColumn.hpp"
#include "util/EntityHelper.hpp"
#include "Iterable.hpp"

/**
 * 通用Example的标准的集合
 */
class Criteria {
private:

    std::vector<Criterion> criteria;//标准的集合
    //属性和列对应
    std::map<std::string, EntityColumn> *propertyMap = nullptr;
    //连接条件
    std::string andOr;
    EntityTable *table = nullptr;//该实体类对应的表

private:

    /**
     * 类的属性值转列名
     * @param property
     * @return
     */
    std::string column(const std::string &property) const {
        if (propertyMap->count(property) > 0) {
            return propertyMap->at(property).getColumnWithTableAlias();
        } else {
            //属性不存在,抛异常
            throw MapperException("[property]" + property + "is not exist!");
        }
    }

    /**
     * 属性类型,可以是成员在对象中的偏移量
     * @param property
     * @param compareSql
     * @param isOffsetType 是否是偏移量类型: &A::a
     * @return
     */
    std::string condition(const std::string &property, const std::string &compareSql) {
        return column(property) + " " + compareSql;
    }

    /**
     * 可以通过成员在对象中的偏移量获取
     * @tparam T
     * @tparam Entity
     * @param propertyPtr
     * @param compareSql
     * @return
     */
    template<typename T, typename Entity>
    std::string condition(T Entity::* propertyPtr, const std::string &compareSql) {
        auto property = EntityHelper::getProperty(propertyPtr);
        return column(property) + " " + compareSql;
    }


//    /**
//     * 判断属性值是否存在等
//     * @param property
//     * @return
//     */
//    const std::string &property(const std::string &property) const {
//        if (propertyMap->count(property) > 0) {
//            return property;
//        } else {
//            throw MapperException("[property]" + property + "is not exist!");
//        }
//    }

public:
    explicit Criteria(std::map<std::string, EntityColumn> *propertyMap, EntityTable *table)
            : propertyMap(propertyMap), table(table) {}

    void setAndOr(const std::string &andOr) {
        this->andOr = andOr;
    }

    const std::string &getAndOr() const {
        return this->andOr;
    }

    const std::vector<Criterion> &getCriteria() const {
        return criteria;
    }

public:
    ////////////////////// and ///////////////////////////////
    template<typename Property>
    Criteria *andIsNull(const Property &property) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::IS_NULL)));
        return this;
    }

    template<typename Property>
    Criteria *andIsNotNull(const Property &property) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::IS_NOT_NULL)));
        return this;
    }

    template<typename Property>
    Criteria *andEqualTo(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::EQUALl_TO), value));
        return this;
    }

    template<typename Property>
    Criteria *andNotEqualTo(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_EQUALl_TO), value));
        return this;
    }

    template<typename Property>
    Criteria *andGreaterThan(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::GREATER_THAN), value));
        return this;
    }

    template<typename Property>
    Criteria *andGreaterThanOrEqualTo(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::GREATER_THAN_OR_EQUAL_TO), value));
        return this;
    }

    template<typename Property>
    Criteria *andLessThan(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::LESS_THAN), value));
        return this;
    }

    template<typename Property>
    Criteria *andLessThanOrEqualTo(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::LESS_THAN_OR_EQUAL_TO), value));
        return this;
    }

    template<typename Property>
    Criteria *andIn(const Property &property, const Iterable &values) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::IN), values));
        return this;
    }

    template<typename Property>
    Criteria *andNotIn(const Property &property, const Iterable &values) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_IN), values));
        return this;
    }

    template<typename Property>
    Criteria *andBetween(const Property &property, const Object &value1, const Object &value2) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::BETWEEN), value1, value2));
        return this;
    }

    template<typename Property>
    Criteria *andNotBetween(const Property &property, const Object &value1, const Object &value2) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_BETWEEN), value1, value2));
        return this;
    }

    template<typename Property>
    Criteria *andLike(const Property &property, const std::string &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::LIKE), value));
        return this;
    }

    template<typename Property>
    Criteria *andNotLike(const Property &property, const std::string &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_LIKE), value));
        return this;
    }


    template<typename Property>
    Criteria *andRegexp(const Property &property, std::string &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::REGEXP), value));
        return this;
    }

    template<typename Property>
    Criteria *andNotRegexp(const Property &property, std::string &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_REGEXP), value));
        return this;
    }

    /**
     * 手写条件
     *
     * @param condition 例如 "length(countryname)<5"
     * @return
     */
    Criteria *andCondition(const std::string &condition) {
        criteria.emplace_back(Criterion(condition));
        return this;
    }

    /**
     * 手写左边条件，右边用value值
     *
     * @param condition 例如 "length(countryname)="
     * @param value     例如 5
     * @return
     */
    Criteria *andCondition(const std::string &condition, const Object &value) {
        criteria.emplace_back(Criterion(condition, value));
        return this;
    }


    /**
     * 将此对象的不为空的字段参数作为相等查询条件
     *
     * @param param 参数对象
     */
    template<typename Object>
    Criteria *andEqualTo(const Object &param) {
        //获取该对象的信息
        std::shared_ptr<EntityTableMap> resultMap = std::make_shared<EntityTableMap>();
        //需要转为Object,不然会变成空类型
        EntityHelper::getResultMap(const_cast<Object *>(&param), resultMap);
        for (auto &property :resultMap->getPropertyMap()) {
            //如果是普通类型,便添加
            if (property.second.getJoinType() != JoinType::OneToMany
                && property.second.getTableAlias() == table->getAlias()) {
                //只包含本表的数据
                this->andEqualTo(property.second.getProperty(), property.second.getEntityFieldValue());
            }
        }
        return this;
    }


    ////////////////////// or ///////////////////////////////
    template<typename Property>
    Criteria *orIsNull(const Property &property) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::IS_NULL), true));
        return this;
    }

    template<typename Property>
    Criteria *orIsNotNull(const Property &property) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::IS_NOT_NULL), true));
        return this;
    }

    template<typename Property>
    Criteria *orEqualTo(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::EQUALl_TO), value, true));
        return this;
    }

    template<typename Property>
    Criteria *orNotEqualTo(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_EQUALl_TO), value, true));
        return this;
    }

    template<typename Property>
    Criteria *orGreaterThan(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::GREATER_THAN), value, true));
        return this;
    }

    template<typename Property>
    Criteria *orGreaterThanOrEqualTo(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::GREATER_THAN_OR_EQUAL_TO), value, true));
        return this;
    }

    template<typename Property>
    Criteria *orLessThan(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::LESS_THAN), value, true));
        return this;
    }

    template<typename Property>
    Criteria *orLessThanOrEqualTo(const Property &property, const Object &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::LESS_THAN_OR_EQUAL_TO), value, true));
        return this;
    }

    template<typename Property>
    Criteria *orIn(const Property &property, const Iterable &values) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::IN), values, true));
        return this;
    }

    template<typename Property>
    Criteria *orNotIn(const Property &property, const Iterable &values) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_IN), values, true));
        return this;
    }

    template<typename Property>
    Criteria *orBetween(const Property &property, const Object &value1, const Object &value2) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::BETWEEN), value1, value2, true));
        return this;
    }

    template<typename Property>
    Criteria *orNotBetween(const Property &property, const Object &value1, const Object &value2) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_BETWEEN), value1, value2, true));
        return this;
    }

    template<typename Property>
    Criteria *orLike(const Property &property, std::string &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::LIKE), value, true));
        return this;
    }

    template<typename Property>
    Criteria *orNotLike(const Property &property, std::string &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_LIKE), value, true));
        return this;
    }

    template<typename Property>
    Criteria *orRegexp(const Property &property, std::string &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::REGEXP), value, true));
        return this;
    }

    template<typename Property>
    Criteria *orNotRegexp(const Property &property, std::string &value) {
        criteria.emplace_back(Criterion(condition(property, SQLConstants::NOT_REGEXP), value, true));
        return this;
    }

    /**
     * 手写条件
     *
     * @param condition 例如 "length(countryname)<5"
     * @return
     */
    Criteria *orCondition(const std::string &condition) {
        criteria.emplace_back(Criterion(condition, true));
        return this;
    }

    /**
     * 手写左边条件，右边用value值
     *
     * @param condition 例如 "length(countryname)="
     * @param value     例如 5
     * @return
     */
    Criteria *orCondition(const std::string &condition, const Object &value) {
        criteria.emplace_back(Criterion(condition, value, true));
        return this;
    }


};

#endif //MAPPER_CRITERIA_HPP
