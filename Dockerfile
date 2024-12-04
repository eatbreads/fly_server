
# 使用一个支持 C++ 的基础镜像
FROM ubuntu:22.04

# 设置时区（可选）
ENV TZ=Asia/Shanghai
RUN ln -sf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# 更新系统并安装 MySQL 以及其他依赖
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libmysqlclient-dev \
    libssl-dev \
    libhiredis-dev \
    libpthread-stubs0-dev \
    mysql-server \
    python3 \
    python3-pip \
    python3-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# 设置 MySQL 密码和其他配置
ENV MYSQL_ROOT_PASSWORD=sjh123456

# 创建数据库初始化目录并复制 SQL 文件
COPY init.sql /docker-entrypoint-initdb.d/
# 将本地的 mysqld.cnf 文件复制到容器的 MySQL 配置目录
COPY mysqld.cnf /etc/mysql/mysql.conf.d/mysqld.cnf

# 将当前目录下的代码复制到镜像中
WORKDIR /app
COPY . /app

# 创建构建目录并构建项目
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make     
# 给脚本赋予执行权限
RUN chmod +x /app/start.sh && \
    /app/start.sh

# 暴露 Web 服务的端口和 MySQL 的默认端口
EXPOSE 8080
EXPOSE 3306

# 启动 MySQL 服务和 Web 服务
# CMD service mysql start && \
#     sleep 10 && \
#     /app/build/main
