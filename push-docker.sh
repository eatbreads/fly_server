#!/bin/bash

# 参数校验
if [ $# -ne 1 ]; then
    echo "Usage: $0 <version>"
    exit 1
fi

VERSION=$1
REPO="eatbreads/mywebproject"

# 构建镜像
docker build -t mywebproject:$VERSION .

# 标记镜像
docker tag mywebproject:$VERSION $REPO:$VERSION
docker tag mywebproject:$VERSION $REPO:latest

# 推送镜像
docker push $REPO:$VERSION
docker push $REPO:latest

echo "Pushed $REPO:$VERSION and updated latest."
