# 使用一个支持 C++ 的基础镜像
FROM ubuntu:22.04

# 设置时区（可选）
ENV TZ=Asia/Shanghai
RUN ln -sf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# 更新系统并安装最新版本的 OpenSSL 和其他依赖
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    libmysqlclient-dev \
    libssl-dev \
    libhiredis-dev \
    libpthread-stubs0-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# 将当前目录下的代码复制到镜像中
WORKDIR /app
COPY . /app


# # 添加调试步骤：查找 MySQL 库和头文件路径
# RUN echo "Searching for libmysqlclient.so:" && find /usr -name "libmysqlclient.so" || echo "MySQL client library not found"
# RUN echo "Searching for mysql.h:" && find /usr -name "mysql.h" || echo "MySQL header file not found"


# 创建构建目录并构建项目
RUN mkdir -p build && \
    cd build && \
    cmake .. && \
    make

# 暴露 Web 服务的端口（修改为你的服务实际端口）
EXPOSE 8080

# 设置容器启动时执行的命令,注意这里是不受之前的cd build影响的,还会是在/app的目录
# CMD ["./main"]
