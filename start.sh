#!/bin/bash

# 启动 MySQL 服务
service mysql start

# 等待 MySQL 启动
sleep 15

# 执行 init.sql 脚本
mysql -u root -p${MYSQL_ROOT_PASSWORD} < /app/init.sql


